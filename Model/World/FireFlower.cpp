#pragma region INCLUDE
#include "FireFlower.h"
#include "../SMB3.h"
#pragma endregion

void CFireFlower::Load() {
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
	auto spawnNode = prefabDoc.child("Prefab").child("Spawn");
	SPAWN_LIMIT = spawnNode.attribute("SPAWN_LIMIT").as_float();
	SPAWN_SPEED = spawnNode.attribute("SPAWN_SPEED").as_float();
}

void CFireFlower::Start() {
	CGameObject::Start();
}

void CFireFlower::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	HandleAction(elapsedMs);
}

void CFireFlower::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CFireFlower::EAction::SPAWN:
	case CFireFlower::EAction::IDLE: {
		_sprites[SPR_FIRE_FLOWER]->Render(_x, _y);
	} break;

	case CFireFlower::EAction::CONSUMED: {
	} break;

	}
}

#pragma region STATE MACHINE

void CFireFlower::HandleAction(float elapsedMs) {
	switch (_action) {
	case CFireFlower::EAction::SPAWN: {
		Spawn(elapsedMs);
	} break;

	case CFireFlower::EAction::IDLE: {
		Idle(elapsedMs);
	} break;

	case CFireFlower::EAction::CONSUMED: {
		Consumed(elapsedMs);
	} break;

	}
}

void CFireFlower::Spawn(float elapsedMs) {
	switch (_actionStage) {
	case CFireFlower::EActionStage::ENTRY: {
		_spawnLimit = _y + SPAWN_LIMIT;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CFireFlower::EActionStage::PROGRESS: {
		if (_y < _spawnLimit) {
			_y += SPAWN_SPEED * elapsedMs;
		}
		else {
			SetNextAction(EAction::IDLE);
			break;
		}
	} break;

	case CFireFlower::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CFireFlower::Idle(float elapsedMs) {
	switch (_actionStage) {
	case CFireFlower::EActionStage::ENTRY: {
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CFireFlower::EActionStage::PROGRESS: {
	} break;

	case CFireFlower::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CFireFlower::Consumed(float elapsedMs) {
	switch (_actionStage) {
	case CFireFlower::EActionStage::ENTRY: {
		Destroy();
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CFireFlower::EActionStage::PROGRESS: {
	} break;

	case CFireFlower::EActionStage::EXIT: {
		NextAction();
	} break;
	
	}
}

#pragma endregion

#pragma region COLLISION

int CFireFlower::IsCollidable() {
	switch (_action) {
	case CFireFlower::EAction::IDLE: {
		return 1;
	} break;

	case CFireFlower::EAction::SPAWN:
	case CFireFlower::EAction::CONSUMED: {
		return 0;
	} break;

	default: {
		return 0;
	} break;
	
	}
}

int CFireFlower::IsBlocking() {
	return 0;
}

void CFireFlower::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (_action) {
	case CFireFlower::EAction::IDLE: {
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	} break;

	case CFireFlower::EAction::SPAWN:
	case CFireFlower::EAction::CONSUMED: { 
	} break;

	}
}

void CFireFlower::OnNoCollision(float elapsedMs){
}

void CFireFlower::OnCollisionWith(pCollision collision){
}

#pragma endregion