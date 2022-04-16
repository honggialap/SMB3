#pragma region INCLUDE
#include "Goomba.h"
#include "../SMB3.h"

#include "Platform.h"
#pragma endregion

void CGoomba::Load() {
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

	/* Type */
	auto typeNode = prefabDoc.child("Prefab").child("Type");
	_type = EType(typeNode.attribute("type").as_int());
	_wing = typeNode.attribute("wing").as_bool();

	/* Gravity */
	auto gravityNode = prefabDoc.child("Prefab").child("Gravity");
	GRAVITY = gravityNode.attribute("GRAVITY").as_float();

	/* Move */
	auto moveNode = prefabDoc.child("Prefab").child("Move");
	WALK_SPEED = moveNode.attribute("WALK_SPEED").as_float();

	/* Jump */
	auto jumpNode = prefabDoc.child("Prefab").child("Jump");
	SMALL_JUMP_FORCE = jumpNode.attribute("SMALL_JUMP_FORCE").as_float();
	LARGE_JUMP_FORCE = jumpNode.attribute("LARGE_JUMP_FORCE").as_float();
	JUMP_INTERVAL = jumpNode.attribute("JUMP_INTERVAL").as_float();

	/* Fly */
	auto flyNode = prefabDoc.child("Prefab").child("Fly");
	TAKE_OFF_SPEED = flyNode.attribute("TAKE_OFF_SPEED").as_float();
	FLY_TIMEOUT = flyNode.attribute("FLY_TIMEOUT").as_float();
	CHANGE_DIRECTION_COUNTDOWN = flyNode.attribute("CHANGE_DIRECTION_COUNTDOWN").as_float();
	TARGET_FLY_OFFSET = flyNode.attribute("TARGET_FLY_OFFSET").as_float();

	/* Hit */
	auto hitNode = prefabDoc.child("Prefab").child("Hit");
	HORIZONTAL_DEFLECT_FORCE = hitNode.attribute("HORIZONTAL_DEFLECT_FORCE").as_float();
	VERTICAL_DEFLECT_FORCE = hitNode.attribute("VERTICAL_DEFLECT_FORCE").as_float();
	DECAY_TIMEOUT = hitNode.attribute("DECAY_TIMEOUT").as_float();
	_targetName = hitNode.attribute("targetName").as_string();
}

void CGoomba::Start() {
	CGameObject::Start();
}

void CGoomba::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	UpdateGravity(elapsedMs);

	std::vector<pGameObject> collidables = _game->GetLocal(_ID);
	_collider->Process(elapsedMs, &collidables);

	HandleAction(elapsedMs);
}

void CGoomba::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CGoomba::EAction::MOVE: {
		if (_wing) {
			_sprites[SPR_GOOMBA_WING1]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::SMALL_JUMP: {
		if (_wing) {
			_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::LARGE_JUMP: {
		if (_wing) {
			_animations[ANI_GOOMBA_WING_FLAP_FAST]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::TAKEOFF: {
		if (_wing) {
			_animations[ANI_GOOMBA_WING_FLAP_FAST]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::FLY: {
		if (_wing) {
			_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::LAND: {
		if (_wing) {
			_sprites[SPR_GOOMBA_WING2]->Render(_x, _y);
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::DIE: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_sprites[SPR_GOOMBA_DIE]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_sprites[SPR_RED_GOOMBA_DIE]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::THROWN: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_DOWN]->Render(_x, _y);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_DOWN]->Render(_x, _y);
		} break;

		}
	} break;

	case CGoomba::EAction::DEADZONED: {
	} break;

	}
}

#pragma region STATE MACHINE

void CGoomba::HandleAction(float elapsedMs) {
	switch (_action) {
	case CGoomba::EAction::MOVE: {
		Move(elapsedMs);
	} break;

	case CGoomba::EAction::SMALL_JUMP: {
		SmallJump(elapsedMs);
	} break;

	case CGoomba::EAction::LARGE_JUMP: {
		LargeJump(elapsedMs);
	} break;

	case CGoomba::EAction::TAKEOFF: {
		TakeOff(elapsedMs);
	} break;

	case CGoomba::EAction::FLY: {
		Fly(elapsedMs);
	} break;

	case CGoomba::EAction::LAND: {
		Land(elapsedMs);
	} break;

	case CGoomba::EAction::DIE: {
		Die(elapsedMs);
	} break;

	case CGoomba::EAction::THROWN: {
		Thrown(elapsedMs);
	} break;

	case CGoomba::EAction::DEADZONED: {
		DeadZoned(elapsedMs);
	} break;

	}
}

void CGoomba::Move(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_jumpInterval = JUMP_INTERVAL;

		AcquireTarget();

		if (_targetX > _x) {
			_left = false;
		}
		else {
			_left = true;
		}

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		if (_wing) {
			if (_jumpInterval > 0) {
				_jumpInterval -= elapsedMs;
			}
			else {
				_jumpInterval = 0;

				if (_type == EType::RED) {
					SetNextAction(EAction::SMALL_JUMP);
					break;
				}
				else if (_type == EType::BROWN) {
					SetNextAction(EAction::TAKEOFF);
					break;
				}
			}
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}
		NextAction();
	} break;

	}
}

void CGoomba::SmallJump(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Play(true);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		_vy = SMALL_JUMP_FORCE;
		_jumpInterval = JUMP_INTERVAL;
		_ground = false;

		AcquireTarget();

		if (_targetX > _x) {
			_left = false;
		}
		else {
			_left = true;
		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Update(elapsedMs);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		if (_ground) {
			_vy = SMALL_JUMP_FORCE;
			_ground = false;
		}

		if (_wing) {
			if (_jumpInterval > 0) {
				_jumpInterval -= elapsedMs;
			}
			else {
				_jumpInterval = 0;
				SetNextAction(EAction::LARGE_JUMP);
				break;
			}
		}
		else {
			SetNextAction(EAction::MOVE);
			break;
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Stop();

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::LargeJump(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Play(true);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		_vy = LARGE_JUMP_FORCE;
		_ground = false;

		AcquireTarget();

		if (_targetX > _x) {
			_left = false;
		}
		else {
			_left = true;
		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Update(elapsedMs);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		if (!_wing) {
			SetNextAction(EAction::MOVE);
			break;
		}

		if (_ground) {
			SetNextAction(EAction::SMALL_JUMP);
			break;
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Stop();

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::TakeOff(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Play(true);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		_vy = TAKE_OFF_SPEED;
		_ground = false;

		AcquireTarget();

		if (_targetX > _x) {
			_left = false;
		}
		else {
			_left = true;
		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Update(elapsedMs);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		_vy = TAKE_OFF_SPEED;

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		AcquireTarget();

		if (_y > _targetY + TARGET_FLY_OFFSET) {
			SetNextAction(EAction::FLY);
			break;
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		_animations[ANI_GOOMBA_WING_FLAP_FAST]->Stop();

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::Fly(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Play(true);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		AcquireTarget();

		if (_targetX > _x) {
			_left = false;
		}
		else {
			_left = true;
		}

		_flyTimeout = FLY_TIMEOUT;
		_changeDirectionCountdown = CHANGE_DIRECTION_COUNTDOWN;
		_vy = TAKE_OFF_SPEED;
		_spawn = true;

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Update(elapsedMs);

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		if (_wing) {
			if (_flyTimeout > 0) {
				_flyTimeout -= elapsedMs;

				if (_changeDirectionCountdown > 0) {
					_changeDirectionCountdown -= elapsedMs;
				}
				else {
					_changeDirectionCountdown = CHANGE_DIRECTION_COUNTDOWN;
					_vy = -_vy;

					if (_spawn) {
						SpawnMicroGoomba();
						_spawn = !_spawn;
					}
					else {
						_spawn = !_spawn;
					}
				}
			}
			else {
				_flyTimeout = 0;
				SetNextAction(EAction::LAND);
				break;
			}
		}
		else {
			SetNextAction(EAction::MOVE);
			break;
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		_animations[ANI_GOOMBA_WING_FLAP_SLOW]->Stop();

		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::Land(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Play(true);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Play(true);
		} break;

		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Update(elapsedMs);
		} break;

		}

		if (_left) {
			_vx = -WALK_SPEED;
		}
		else {
			_vx = WALK_SPEED;
		}

		if (_ground) {
			SetNextAction(EAction::MOVE);
			break;
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_UP]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_UP]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::Die(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_decayTimeout = DECAY_TIMEOUT;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		if (_decayTimeout > 0) {
			_decayTimeout -= elapsedMs;
		}
		else {
			Destroy();
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CGoomba::Thrown(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_DOWN]->Play(true);
		} break;

		case CGoomba::EType::RED:
			_animations[ANI_RED_GOOMBA_WALK_DOWN]->Play(true);
			break;
		}

		_decayTimeout = DECAY_TIMEOUT;

		if (_left) {
			_vx -= HORIZONTAL_DEFLECT_FORCE;
		}
		else {
			_vx = HORIZONTAL_DEFLECT_FORCE;
		}

		_vy = VERTICAL_DEFLECT_FORCE;

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_DOWN]->Update(elapsedMs);
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_DOWN]->Update(elapsedMs);
		} break;

		}

		if (_decayTimeout > 0) {
			_decayTimeout -= elapsedMs;
		}
		else {
			Destroy();
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		switch (_type) {
		case CGoomba::EType::BROWN: {
			_animations[ANI_GOOMBA_WALK_DOWN]->Stop();
		} break;

		case CGoomba::EType::RED: {
			_animations[ANI_RED_GOOMBA_WALK_DOWN]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CGoomba::DeadZoned(float elapsedMs) {
	switch (_actionStage) {
	case CGoomba::EActionStage::ENTRY: {
		_decayTimeout = DECAY_TIMEOUT;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CGoomba::EActionStage::PROGRESS: {
		if (_decayTimeout > 0) {
			_decayTimeout -= elapsedMs;
		}
		else {
			Destroy();
		}
	} break;

	case CGoomba::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CGoomba::UpdateGravity(float elapsedMs) {
	if (_action != EAction::DEADZONED
		&& _action != EAction::FLY) {
		_vy -= GRAVITY * elapsedMs;
	}
}

void CGoomba::HitTop() {
	if (_wing) {
		_wing = false;
	}
	else {
		SetNextAction(EAction::DIE);
	}
}

void CGoomba::HitSide(bool left) {
	if (left) {
		_vx = 0;
	}
	else {
		_vx = 0;
	}

	_vy = 0;
	_wing = false;

	SetNextAction(EAction::THROWN);
}

void CGoomba::AcquireTarget() {
	auto target = _game->GetGameObject(_targetName);
	if (target != nullptr) {
		target->GetPosition(_targetX, _targetY);
	}
}

void CGoomba::SpawnMicroGoomba() {
}

#pragma endregion

#pragma region COLLISION

int CGoomba::IsCollidable() {
	switch (_action) {
	case CGoomba::EAction::MOVE:
	case CGoomba::EAction::SMALL_JUMP:
	case CGoomba::EAction::LARGE_JUMP:
	case CGoomba::EAction::TAKEOFF:
	case CGoomba::EAction::FLY:
	case CGoomba::EAction::LAND:
	case CGoomba::EAction::DIE: {
		return 1;
	} break;

	case CGoomba::EAction::THROWN:
	case CGoomba::EAction::DEADZONED: {
		return 0;
	} break;

	default: {
		return 0;
	} break;

	}
}

int CGoomba::IsBlocking() {
	return 0;
}

void CGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	switch (_action) {
	case CGoomba::EAction::MOVE:
	case CGoomba::EAction::SMALL_JUMP:
	case CGoomba::EAction::LARGE_JUMP:
	case CGoomba::EAction::TAKEOFF:
	case CGoomba::EAction::FLY:
	case CGoomba::EAction::LAND:
	case CGoomba::EAction::DIE: {
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	} break;

	}
}

void CGoomba::OnNoCollision(float elapsedMs) {
	_x += _vx * elapsedMs;
	_y += _vy * elapsedMs;
}

void CGoomba::OnCollisionWith(pCollision collision) {
	if (dynamic_cast<pPlatform>(collision->_target)) {
		OnCollisionWithPlatform(collision);
	}
}

void CGoomba::OnCollisionWithPlatform(pCollision collision) {
	auto platform = dynamic_cast<pPlatform>(collision->_target);
	if (platform->_solid) {
		if (collision->_ny != 0 && collision->_target->IsBlocking()) {
			_vy = 0;
			if (collision->_ny > 0) {
				_ground = true;
			}
		}

		if (collision->_nx != 0 && collision->_target->IsBlocking()) {
			_vx = 0;
			_left = !_left;
		}
	}
	else {
		if (collision->_ny > 0 && !collision->_target->IsBlocking()) {
			float top = 0;
			float temp = 0;
			collision->_target->GetBoundingBox(temp, top, temp, temp);

			_y = top + BLOCK_PUSH_FACTOR;
			_vy = 0;
			_ground = true;
		}
	}
}

#pragma endregion