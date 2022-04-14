#pragma region INCLUDE
#include "Collision.h"
#include "GameObject.h"
#pragma endregion

CCollider::CCollider(
	pGameObject gameObject
) {
	_gameObject = gameObject;
}

CCollider::~CCollider() {
	_gameObject = nullptr;
}

void CCollider::SweptAABB(
	float ml, float mt, float mr, float mb,
	float dx, float dy,
	float sl, float st, float sr, float sb,
	float& t,
	float& nx, float& ny
) {
	float dx_entry, dx_exit, tx_entry, tx_exit;
	float dy_entry, dy_exit, ty_entry, ty_exit;

	float t_entry;
	float t_exit;

	t = -1.0f;			// no collision
	nx = ny = 0;

	//
	// Broad-phase test 
	//

	float bl = dx > 0 ? ml : ml + dx;
	float br = dx > 0 ? mr + dx : mr;
	float bt = dy > 0 ? mt + dy : mt;
	float bb = dy > 0 ? mb : mb + dy;

	if (br < sl || bl > sr || bb > st || bt < sb) return;

	if (dx == 0 && dy == 0) return;		// moving object is not moving > obvious no collision

	if (dx > 0) {
		dx_entry = sl - mr;
		dx_exit = sr - ml;
	}
	else if (dx < 0) {
		dx_entry = sr - ml;
		dx_exit = sl - mr;
	}


	if (dy > 0) {
		dy_entry = sb - mt;
		dy_exit = st - mb;
	}
	else if (dy < 0) {
		dy_entry = st - mb;
		dy_exit = sb - mt;
	}

	if (dx == 0) {
		tx_entry = -9999999.0f;
		tx_exit = 99999999.0f;
	}
	else {
		tx_entry = dx_entry / dx;
		tx_exit = dx_exit / dx;
	}

	if (dy == 0) {
		ty_entry = -99999999999.0f;
		ty_exit = 99999999999.0f;
	}
	else {
		ty_entry = dy_entry / dy;
		ty_exit = dy_exit / dy;
	}


	if ((tx_entry < 0.0f && ty_entry < 0.0f) || tx_entry > 1.0f || ty_entry > 1.0f) return;

	t_entry = max(tx_entry, ty_entry);
	t_exit = min(tx_exit, ty_exit);

	if (t_entry > t_exit) return;

	t = t_entry;

	if (tx_entry > ty_entry) {
		ny = 0.0f;
		dx > 0 ? nx = -1.0f : nx = 1.0f;
	}
	else {
		nx = 0.0f;
		dy > 0 ? ny = -1.0f : ny = 1.0f;
	}
}

pCollision CCollider::Overlap(
	pGameObject collidable
) {
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t = -1.0f;
	_gameObject->GetBoundingBox(ml, mt, mr, mb);
	collidable->GetBoundingBox(sl, st, sr, sb);

	if (sr < ml || mr < sl) {
		t = -1.0f;
	}
	else if (mb > st || sb > mt) {
		t = -1.0f;
	}
	else {
		t = 0.0f;
	}

	pCollision e = new CCollision(t, 0, 0, 0, 0, true, collidable);
	return e;
}

void CCollider::ScanOverlap(
	std::vector<pGameObject>* collidables
) {
	for (UINT i = 0; i < collidables->size(); i++) {
		pCollision e = Overlap(collidables->at(i));

		if (e->WasCollided() == 1) {
			_collisions.push_back(e);
		}
		else {
			delete e;
		}
	}
}

pCollision CCollider::SweptAABB(
	float elapsedMs,
	pGameObject collidable
) {
	float sl, st, sr, sb;		// static object bbox
	float ml, mt, mr, mb;		// moving object bbox
	float t, nx, ny;

	float mvx, mvy;
	_gameObject->GetVelocity(mvx, mvy);
	float mdx = mvx * elapsedMs;
	float mdy = mvy * elapsedMs;

	float svx, svy;
	collidable->GetVelocity(svx, svy);
	float sdx = svx * elapsedMs;
	float sdy = svy * elapsedMs;

	//
	// NOTE: new m speed = original m speed - collide object speed
	// 
	float dx = mdx - sdx;
	float dy = mdy - sdy;

	_gameObject->GetBoundingBox(ml, mt, mr, mb);
	collidable->GetBoundingBox(sl, st, sr, sb);

	SweptAABB(
		ml, mt, mr, mb,
		dx, dy,
		sl, st, sr, sb,
		t, nx, ny
	);

	pCollision e = new CCollision(t, nx, ny, dx, dy, false, collidable);
	return e;
}

void CCollider::ScanSweptAABB(
	float elapsedMs,
	std::vector<pGameObject>* collidables
) {
	for (UINT i = 0; i < collidables->size(); i++) {
		pCollision e = SweptAABB(elapsedMs, collidables->at(i));

		if (e->WasCollided() == 1)
			_collisions.push_back(e);
		else
			delete e;
	}

	std::sort(_collisions.begin(), _collisions.end(), CCollision::CompareTime);
}

void CCollider::Filter(
	pCollision& colX, pCollision& colY,
	int filterBlock,
	int filterX, int filterY
) {
	float min_tx, min_ty;

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	for (UINT i = 0; i < _collisions.size(); i++) {
		pCollision c = _collisions[i];
		if (c->_destroy) continue;
		if (c->_target->IsDestroyed()) continue;

		// ignore collision event with object having IsBlocking = 0 (like coin, mushroom, etc)
		if (filterBlock == 1 && !c->_target->IsBlocking()) {
			continue;
		}

		if (c->_t < min_tx && c->_nx != 0 && filterX == 1) {
			min_tx = c->_t; min_ix = i;
		}

		if (c->_t < min_ty && c->_ny != 0 && filterY == 1) {
			min_ty = c->_t; min_iy = i;
		}
	}

	if (min_ix >= 0) colX = _collisions[min_ix];
	if (min_iy >= 0) colY = _collisions[min_iy];
}

void CCollider::Process(
	float elapsedMs,
	std::vector<pGameObject>* collidables
) {
	_collisions.clear();

	// Overlapping
	if (_gameObject->IsCollidable()) {
		ScanOverlap(collidables);
	}
	if (_collisions.size() != 0) {
		for (UINT i = 0; i < _collisions.size(); i++) {
			pCollision e = _collisions[i];
			if (e->_destroy) continue;
			if (e->_overlap) _gameObject->OnCollisionWith(e);
			if (e->_target->IsBlocking()) continue;  // blocking collisions will be handled later, skip them
		}
		for (UINT i = 0; i < _collisions.size(); i++) delete _collisions[i];
	}

	_collisions.clear();

	pCollision colX = NULL;
	pCollision colY = NULL;

	// Sweeping
	if (_gameObject->IsCollidable()) {
		ScanSweptAABB(elapsedMs, collidables);
	}

	if (_collisions.size() == 0) {
		_gameObject->OnNoCollision(elapsedMs);
	}
	else {
		Filter(colX, colY);

		float x, y, vx, vy, dx, dy;
		_gameObject->GetPosition(x, y);
		_gameObject->GetVelocity(vx, vy);
		dx = vx * elapsedMs;
		dy = vy * elapsedMs;

		if (colX != NULL && colY != NULL) {
			if (colY->_t < colX->_t) {	// was collision on Y first ?
				y += colY->_t * dy + colY->_ny * BLOCK_PUSH_FACTOR;
				_gameObject->SetPosition(x, y);

				_gameObject->OnCollisionWith(colY);

				//
				// see if after correction on Y, is there still a collision on X ? 
				//
				pCollision colX_other = NULL;

				//
				// check again if there is true collision on X 
				//
				colX->_destroy = true;		// remove current collision event on X

				// replace with a new collision event using corrected location 
				_collisions.push_back(SweptAABB(elapsedMs, colX->_target));

				// re-filter on X only
				Filter(
					colX_other,
					colY,
					1,	//filterBlock
					1,	//filterX
					0	//filterY
				);

				if (colX_other != NULL) {
					x += colX_other->_t * dx + colX_other->_nx * BLOCK_PUSH_FACTOR;
					_gameObject->OnCollisionWith(colX_other);
				}
				else {
					x += dx;
				}
			}
			else { // collision on X first

				x += colX->_t * dx + colX->_nx * BLOCK_PUSH_FACTOR;
				_gameObject->SetPosition(x, y);

				_gameObject->OnCollisionWith(colX);

				//
				// see if after correction on X, is there still a collision on Y ? 
				//
				pCollision colY_other = NULL;

				//
				// check again if there is true collision on Y
				//
				colY->_destroy = true;		// remove current collision event on Y

				// replace with a new collision event using corrected location 
				_collisions.push_back(SweptAABB(elapsedMs, colY->_target));

				// re-filter on Y only
				Filter(
					colX,
					colY_other,
					1, //filterBlock
					0, //filterX
					1  //filterY
				);

				if (colY_other != NULL) {
					y += colY_other->_t * dy + colY_other->_ny * BLOCK_PUSH_FACTOR;
					_gameObject->OnCollisionWith(colY_other);
				}
				else {
					y += dy;
				}
			}
		}
		else {
			if (colX != NULL) {
				x += colX->_t * dx + colX->_nx * BLOCK_PUSH_FACTOR;
				y += dy;
				_gameObject->OnCollisionWith(colX);
			}
			else if (colY != NULL) {
				x += dx;
				y += colY->_t * dy + colY->_ny * BLOCK_PUSH_FACTOR;
				_gameObject->OnCollisionWith(colY);
			}
			else { // both colX & colY are NULL 
				x += dx;
				y += dy;
			}
		}

		_gameObject->SetPosition(x, y);
	}

	//
	// Scan all non-blocking collisions for further collision logic
	//
	for (UINT i = 0; i < _collisions.size(); i++) {
		pCollision e = _collisions[i];
		if (e->_destroy) continue;
		if (e->_target->IsBlocking()) continue;  // blocking collisions were handled already, skip them

		_gameObject->OnCollisionWith(e);
	}

	for (UINT i = 0; i < _collisions.size(); i++) delete _collisions[i];
}