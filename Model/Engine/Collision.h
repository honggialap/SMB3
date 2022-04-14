#pragma once
#ifndef __COLLISION_H__
#define __COLLISION_H__

#pragma region FORWARD DECLARATION
class CGameObject;
typedef CGameObject* pGameObject;
#pragma endregion

#pragma region DEFINE
#define BLOCK_PUSH_FACTOR 0.4f
#pragma endregion

#pragma region INCLUDE
#include <vector>
#include <algorithm>
#include <Windows.h>
#pragma endregion

struct CCollision {
	pGameObject _target;
	float _t, _nx, _ny;
	float _dx, _dy;				// *RELATIVE* movement distance between this object and obj
	bool _overlap;
	bool _destroy;

	CCollision(
		float t, float nx, float ny,
		float dx, float dy,
		bool overlap,
		pGameObject target
	) {
		_t = t;
		_nx = nx;
		_ny = ny;
		_dx = dx;
		_dy = dy;
		_overlap = overlap;
		_target = target;
		_destroy = false;
	}

	bool WasCollided() { return _t >= 0.0f && _t <= 1.0f; }
	static bool CompareTime(CCollision*& a, CCollision*& b) {
		return a->_t < b->_t;
	}
};
typedef CCollision* pCollision;

class CCollider {
private:
	pGameObject _gameObject;
	std::vector<pCollision> _collisions;

public:
	CCollider(pGameObject gameObject);
	~CCollider();

	static void SweptAABB(
		float ml,			// move left
		float mt,			// move top
		float mr,			// move right
		float mb,			// move bottom
		float dx,
		float dy,
		float sl,			// static left
		float st,			// static top
		float sr,			// static right
		float sb,			// static bottom
		float& t,
		float& nx,
		float& ny
	);

	pCollision Overlap(pGameObject collidable);
	void ScanOverlap(std::vector<pGameObject>* collidables);

	pCollision SweptAABB(float elapsedMs, pGameObject collidable);
	void ScanSweptAABB(float elapsedMs, std::vector<pGameObject>* collidables);

	void Filter(
		pCollision& colX,
		pCollision& colY,
		int filterBlock = 1,
		int filterX = 1,
		int filterY = 1
	);

	void Process(
		float elapsedMs,
		std::vector<pGameObject>* collidables
	);
};
typedef CCollider* pCollider;

#endif // !__COLLISION_H__
