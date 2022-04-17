#pragma region INCLUDE
#include "Coin.h"
#include "../SMB3.h"
#pragma endregion

void CCoin::Load() {
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

	/* Spin */
	auto spinNode = prefabDoc.child("Prefab").child("Spin");
	GRAVITY = spinNode.attribute("GRAVITY").as_float();
	BOUNCE_FORCE = spinNode.attribute("BOUNCE_FORCE").as_float();
	DECAY_INTERVAL = spinNode.attribute("DECAY_INTERVAL").as_float();
	_spin = spinNode.attribute("spin").as_bool();
}

void CCoin::Start() {
	CGameObject::Start();
}

void CCoin::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	HandleAction(elapsedMs);
}

void CCoin::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CCoin::EAction::IDLE: {
		if (_spin) {
			_animations[ANI_COIN_SPIN_SLOW]->Render(_x, _y);
		}
		else {
			_sprites[SPR_COIN1]->Render(_x, _y);
		}
	} break;

	case CCoin::EAction::CONSUMED: {
		_animations[ANI_COIN_SPIN_FAST]->Render(_x, _y);
	} break;

	}
}

#pragma region STATE MACHINE

void CCoin::HandleAction(float elapsedMs) {
	switch (_action) {
	case CCoin::EAction::IDLE: {
		Idle(elapsedMs);
	} break;

	case CCoin::EAction::CONSUMED: {
		Consumed(elapsedMs);
	} break;

	}
}

void CCoin::Idle(float elapsedMs) {
	switch (_actionStage) {
	case CCoin::EActionStage::ENTRY: {
		_animations[ANI_COIN_SPIN_SLOW]->Play(true);
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CCoin::EActionStage::PROGRESS: {
		_animations[ANI_COIN_SPIN_SLOW]->Update(elapsedMs);
	} break;

	case CCoin::EActionStage::EXIT: {
		_animations[ANI_COIN_SPIN_SLOW]->Stop();
		NextAction();
	} break;

	}
}

void CCoin::Consumed(float elapsedMs) {
	switch (_actionStage) {
	case CCoin::EActionStage::ENTRY: {
		_animations[ANI_COIN_SPIN_FAST]->Play(true);
		_decayInterval = DECAY_INTERVAL;
		_vy = BOUNCE_FORCE;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CCoin::EActionStage::PROGRESS: {
		_animations[ANI_COIN_SPIN_FAST]->Update(elapsedMs);

		if (_decayInterval > 0) {
			_decayInterval -= elapsedMs;
			_vy -= GRAVITY * elapsedMs;
		}
		else {
			_animations[ANI_COIN_SPIN_FAST]->Stop();
			Destroy();
		}

		_x += _vx * elapsedMs;
		_y += _vy * elapsedMs;
	} break;

	case CCoin::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

#pragma endregion

#pragma region COLLISION

int CCoin::IsCollidable() {
	switch (_action) {
	case CCoin::EAction::IDLE: {
		return 1;
	} break;

	case CCoin::EAction::CONSUMED: {
		return 0;
	} break;

	default: {
		return 0;
	} break;

	}
}

int CCoin::IsBlocking() {
	return 0;
}

void CCoin::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (_action) {
	case CCoin::EAction::IDLE: {
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	} break;

	case CCoin::EAction::CONSUMED: {
	} break;
	
	}
}

void CCoin::OnNoCollision(float elapsedMs) {
}

void CCoin::OnCollisionWith(pCollision collision) {
}

#pragma endregion