#pragma region INCLUDE
#include "Brick.h"
#include "../SMB3.h"
#pragma endregion

void CBrick::Load() {
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

	/* Bounce */
	auto bounceNode = prefabDoc.child("Prefab").child("Bounce");
	BOUNCE_LIMIT = bounceNode.attribute("BOUNCE_LIMIT").as_float();
	BOUNCE_SPEED = bounceNode.attribute("BOUNCE_SPEED").as_float();
}

void CBrick::Start() {
	CGameObject::Start();
}

void CBrick::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	HandleAction(elapsedMs);
}

void CBrick::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CBrick::EAction::IDLE:
	case CBrick::EAction::BOUNCE: {
		_animations[ANI_BRICK]->Render(_renderX, _renderY);
	} break;

	case CBrick::EAction::BROKE: {
	} break;

	}
}

#pragma region STATE MACHINE

void CBrick::HandleAction(float elapsedMs) {
	switch (_action) {
	case CBrick::EAction::IDLE: {
		Idle(elapsedMs);
	} break;

	case CBrick::EAction::BOUNCE: {
		Bounce(elapsedMs);
	} break;

	case CBrick::EAction::BROKE: {
		Broke(elapsedMs);
	} break;

	}
}

void CBrick::Idle(float elapsedMs) {
	switch (_actionStage) {
	case CBrick::EActionStage::ENTRY: {
		_animations[ANI_BRICK]->Play(true);

		_renderX = _x;
		_renderY = _y;

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBrick::EActionStage::PROGRESS: {
		_animations[ANI_BRICK]->Update(elapsedMs);
	} break;

	case CBrick::EActionStage::EXIT: {
		_animations[ANI_BRICK]->Stop();
		NextAction();
	} break;

	}
}

void CBrick::Bounce(float elapsedMs) {
	switch (_actionStage) {
	case CBrick::EActionStage::ENTRY: {
		_animations[ANI_BRICK]->Play(true);

		_return = false;
		_bounceOrigin = _y;
		_bounceLimit = _y + BOUNCE_LIMIT;
		_renderX = _x;
		_renderY = _bounceOrigin;

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBrick::EActionStage::PROGRESS: {
		_animations[ANI_BRICK]->Update(elapsedMs);

		if (!_return) {
			if (_renderY < _bounceLimit) {
				_renderY += BOUNCE_SPEED * elapsedMs;
			}
			else {
				_renderY = _bounceLimit;
				_return = true;
			}
		}
		else {
			if (_renderY > _bounceOrigin) {
				_renderY -= BOUNCE_SPEED * elapsedMs;
			}
			else {
				_renderY = _bounceOrigin;
				_return = false;
				SetNextAction(EAction::IDLE);
				break;
			}
		}

	} break;

	case CBrick::EActionStage::EXIT: {
		_animations[ANI_BRICK]->Stop();
		NextAction();
	} break;

	}
}

void CBrick::Broke(float elapsedMs) {
	switch (_actionStage) {
	case CBrick::EActionStage::ENTRY: {
		Destroy();
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBrick::EActionStage::PROGRESS: {
	} break;

	case CBrick::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CBrick::HitBottom() {
	if (_action == EAction::IDLE) {
		SetNextAction(EAction::BOUNCE);
	}
}

void CBrick::Brake() {
	if (_action == EAction::IDLE) {
		SetNextAction(EAction::BROKE);
	}
}

#pragma endregion

#pragma region COLLISION

int CBrick::IsCollidable() {
	return 1;
}

int CBrick::IsBlocking() {
	return 1;
}

void CBrick::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CBrick::OnNoCollision(float elapsedMs) {
}

void CBrick::OnCollisionWith(pCollision collision) {
}

#pragma endregion