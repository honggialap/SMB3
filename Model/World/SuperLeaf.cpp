#pragma region INCLUDE
#include "SuperLeaf.h"
#include "../SMB3.h"
#pragma endregion

void CSuperLeaf::Load() {
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefabDoc;
	prefabDoc.load_file(_source.c_str());

	/* Body */
	auto bodyNode = prefabDoc.child("Prefab").child("Body");
	_renderBody = bodyNode.attribute("render").as_bool();
	BODY_WIDTH = bodyNode.attribute("width").as_float();
	BODY_HEIGHT = bodyNode.attribute("height").as_float();
	BODY_OFFSETX = bodyNode.attribute("offsetX").as_float();
	BODY_OFFSETY = bodyNode.attribute("offsetY").as_float();

	/* Move */
	auto moveNode = prefabDoc.child("Prefab").child("Move");
	DROP_SPEED = moveNode.attribute("DROP_SPEED").as_float();
	MOVE_SPEED = moveNode.attribute("MOVE_SPEED").as_float();
	MOVE_LIMIT = moveNode.attribute("MOVE_LIMIT").as_float();
	SPAWN_SPEED = moveNode.attribute("SPAWN_SPEED").as_float();
	SPAWN_LIMIT = moveNode.attribute("SPAWN_LIMIT").as_float();
	_targetName = moveNode.attribute("targetName").as_string();
}

void CSuperLeaf::Start() {
	CGameObject::Start();
}

void CSuperLeaf::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	HandleAction(elapsedMs);
}

void CSuperLeaf::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CSuperLeaf::EAction::SPAWN:
	case CSuperLeaf::EAction::MOVE: {
		if (_left) {
			_sprites[SPR_SUPER_LEAF_LEFT]->Render(_x, _y);
		}
		else {
			_sprites[SPR_SUPER_LEAF_RIGHT]->Render(_x, _y);
		}
	} break;

	case CSuperLeaf::EAction::CONSUMED: {
	} break;

	}
}

#pragma region STATE MACHINE

void CSuperLeaf::HandleAction(float elapsedMs) {
	switch (_action) {
	case CSuperLeaf::EAction::SPAWN: {
		Spawn(elapsedMs);
	} break;

	case CSuperLeaf::EAction::MOVE: {
		Move(elapsedMs);
	} break;

	case CSuperLeaf::EAction::CONSUMED: {
		Consumed(elapsedMs);
	} break;

	}
}

void CSuperLeaf::Spawn(float elapsedMs) {
	switch (_actionStage) {
	case CSuperLeaf::EActionStage::ENTRY: {
		_spawnLimit = _y + SPAWN_LIMIT;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CSuperLeaf::EActionStage::PROGRESS: {
		if (_y < _spawnLimit) {
			_y += SPAWN_SPEED * elapsedMs;
		}
		else {
			SetNextAction(EAction::MOVE);
			break;
		}
	} break;

	case CSuperLeaf::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CSuperLeaf::Move(float elapsedMs) {
	switch (_actionStage) {
	case CSuperLeaf::EActionStage::ENTRY: {
		_moveOrigin = _x - MOVE_LIMIT / 2;
		_moveLimit = _x + MOVE_LIMIT / 2;
		_left = false;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CSuperLeaf::EActionStage::PROGRESS: {
		if (_left) {
			_vx = -MOVE_SPEED;
			if (_x < _moveOrigin) _left = false;
		}
		else {
			_vx = MOVE_SPEED;
			if (_x > _moveLimit) _left = true;
		}

		_vy = -DROP_SPEED;

		_x += _vx * elapsedMs;
		_y += _vy * elapsedMs;
	} break;

	case CSuperLeaf::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CSuperLeaf::Consumed(float elapsedMs) {
	switch (_actionStage) {
	case CSuperLeaf::EActionStage::ENTRY: {
		Destroy();
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CSuperLeaf::EActionStage::PROGRESS: {
	} break;

	case CSuperLeaf::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

#pragma endregion

#pragma region COLLISION

int CSuperLeaf::IsCollidable() {
	switch (_action) {
	case CSuperLeaf::EAction::MOVE: {
		return 1;
	} break;

	case CSuperLeaf::EAction::SPAWN:
	case CSuperLeaf::EAction::CONSUMED: {
		return 0;
	} break;

	default: {
		return 0;
	} break;

	}
}

int CSuperLeaf::IsBlocking() {
	return 0;
}

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (_action) {
	case CSuperLeaf::EAction::MOVE: {
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	} break;

	case CSuperLeaf::EAction::SPAWN:
	case CSuperLeaf::EAction::CONSUMED: {
	} break;

	}
}

void CSuperLeaf::OnNoCollision(float elapsedMs) {
}

void CSuperLeaf::OnCollisionWith(pCollision collision) {
}

#pragma endregion