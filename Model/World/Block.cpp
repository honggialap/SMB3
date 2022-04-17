#pragma region INCLUDE
#include "Block.h"
#include "../SMB3.h"

#include "Mario.h"
#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "FireFlower.h"
#include "ExtraLifeMushroom.h"
#include "Relay.h"
#pragma endregion

void CBlock::Load() {
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
	_spawn = ESpawn(typeNode.attribute("spawn").as_int());
	_spawnCount = typeNode.attribute("spawnCount").as_int();

	/* Bounce */
	auto bounceNode = prefabDoc.child("Prefab").child("Bounce");
	BOUNCE_LIMIT = bounceNode.attribute("BOUNCE_LIMIT").as_float();
	BOUNCE_SPEED = bounceNode.attribute("BOUNCE_SPEED").as_float();
	_targetName = bounceNode.attribute("target").as_string();
}

void CBlock::Start() {
	CGameObject::Start();
}

void CBlock::Update(float elapsedMs) {
	if (!_start) {
		Start();
	}

	HandleAction(elapsedMs);
}

void CBlock::Render() {
	if (_renderBody) {
		_sprites[SPR_BBOX]->Render(_x, _y);
	}

	switch (_action) {
	case CBlock::EAction::IDLE:
	case CBlock::EAction::BOUNCE: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Render(_renderX, _renderY);
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Render(_renderX, _renderY);
		} break;

		}
	} break;

	case CBlock::EAction::EMPTY: {
		_sprites[SPR_EMPTY_BLOCK]->Render(_renderX, _renderY);
	} break;

	}
}

#pragma region STATE MACHINE

void CBlock::HandleAction(float elapsedMs) {
	switch (_action) {
	case CBlock::EAction::IDLE: {
		Idle(elapsedMs);
	} break;

	case CBlock::EAction::BOUNCE: {
		Bounce(elapsedMs);
	} break;

	case CBlock::EAction::EMPTY: {
		Empty(elapsedMs);
	} break;

	}
}

void CBlock::Idle(float elapsedMs) {
	switch (_actionStage) {
	case CBlock::EActionStage::ENTRY: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Play(true);
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Play(true);
		} break;

		}

		_renderX = _x;
		_renderY = _y;
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBlock::EActionStage::PROGRESS: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Update(elapsedMs);
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Update(elapsedMs);
		} break;

		}
	} break;

	case CBlock::EActionStage::EXIT: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Stop();
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Stop();
		} break;

		}
		NextAction();
	} break;

	}
}

void CBlock::Bounce(float elapsedMs) {
	switch (_actionStage) {
	case CBlock::EActionStage::ENTRY: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Play(true);
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Play(true);
		} break;

		}

		_return = false;

		if (_topBounce) {
			_bounceOrigin = _y;
			_bounceLimit = _y + BOUNCE_LIMIT;
			_renderX = _x;
			_renderY = _bounceOrigin;
		}
		else {
			if (_leftBounce) {
				_bounceOrigin = _x;
				_bounceLimit = _x - BOUNCE_LIMIT / 2;
				_renderX = _bounceOrigin;
				_renderY = _y;
			}
			else {
				_bounceOrigin = _x;
				_bounceLimit = _x + BOUNCE_LIMIT / 2;
				_renderX = _bounceOrigin;
				_renderY = _y;
			}
		}

		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBlock::EActionStage::PROGRESS: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Update(elapsedMs);
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Update(elapsedMs);
		} break;

		}

		if (_topBounce) {
			if (!_return) {
				if (_renderY < _bounceLimit) {
					_renderY += BOUNCE_SPEED * elapsedMs;
				}
				else {
					_renderY = _bounceLimit;
					_return = true;
					if (_spawnCount > 0) {
						Spawn();
						_spawnCount--;
					}
				}
			}
			else {
				if (_renderY > _bounceOrigin) {
					_renderY -= BOUNCE_SPEED * elapsedMs;
				}
				else {
					_renderY = _bounceOrigin;
					_return = false;

					if (_spawnCount == 0) {
						SetNextAction(EAction::EMPTY);
						break;
					}

					SetNextAction(EAction::IDLE);
					break;
				}
			}
		}
		else {
			if (_leftBounce) {
				if (!_return) {
					if (_renderX > _bounceLimit) {
						_renderX -= BOUNCE_SPEED * elapsedMs;
					}
					else {
						_renderX = _bounceLimit;
						_return = true;

						if (_spawnCount > 0) {
							Spawn();
							_spawnCount--;
						}
					}
				}
				else {
					if (_renderX < _bounceOrigin) {
						_renderX += BOUNCE_SPEED * elapsedMs;
					}
					else {
						_renderY = _bounceOrigin;
						_return = false;

						if (_spawnCount == 0) {
							SetNextAction(EAction::EMPTY);
							break;
						}

						SetNextAction(EAction::IDLE);
						break;
					}
				}
			}
			else {
				if (!_return) {
					if (_renderX < _bounceLimit) {
						_renderX += BOUNCE_SPEED * elapsedMs;
					}
					else {
						_renderX = _bounceLimit;
						_return = true;

						if (_spawnCount > 0) {
							Spawn();
							_spawnCount--;
						}
					}
				}
				else {
					if (_renderX > _bounceOrigin) {
						_renderX -= BOUNCE_SPEED * elapsedMs;
					}
					else {
						_renderY = _bounceOrigin;
						_return = false;

						if (_spawnCount == 0) {
							SetNextAction(EAction::EMPTY);
							break;
						}

						SetNextAction(EAction::IDLE);
						break;
					}
				}
			}
		}
	} break;

	case CBlock::EActionStage::EXIT: {
		switch (_type) {
		case CBlock::EType::QUESTION: {
			_animations[ANI_QUESTION_BLOCK]->Stop();
		} break;

		case CBlock::EType::BRICK: {
			_animations[ANI_BRICK_BLOCK]->Stop();
		} break;

		}

		NextAction();
	} break;

	}
}

void CBlock::Empty(float elapsedMs) {
	switch (_actionStage) {
	case CBlock::EActionStage::ENTRY: {
		_actionStage = EActionStage::PROGRESS;
	} break;

	case CBlock::EActionStage::PROGRESS: {
	} break;

	case CBlock::EActionStage::EXIT: {
		NextAction();
	} break;

	}
}

void CBlock::Spawn() {
	/* Read file */
	pugi::xml_document prefabDoc;
	prefabDoc.load_file(_source.c_str());

	switch (_spawn) {
	case CBlock::ESpawn::COIN: {
		auto coinNode = prefabDoc.child("Prefab").child("Coin");
		auto spawnedCoin = dynamic_cast<pCoin>(
			_game->Create(
				_scene,
				coinNode.attribute("actorID").as_int(),
				_name + coinNode.attribute("name").as_string(),
				coinNode.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedCoin->SetAction(CCoin::EAction::CONSUMED);
	} break;

	case CBlock::ESpawn::POWER_UP: {
		if (_game->GetGameObject(_targetName) != nullptr) {
			auto mario = dynamic_cast<pMario>(_game->GetGameObject(_targetName));
			if (mario->_power == CMario::EPower::SMALL) {
				auto superMushroomNode = prefabDoc.child("Prefab").child("SuperMushroom");
				auto spawnedSuperMushroom = dynamic_cast<pSuperMushroom>(
					_game->Create(
						_scene,
						superMushroomNode.attribute("actorID").as_int(),
						_name + superMushroomNode.attribute("name").as_string(),
						superMushroomNode.attribute("source").as_string(),
						_x, _y, _gx, _gy, _layer - 1
					)
					);
				spawnedSuperMushroom->SetAction(CSuperMushroom::EAction::SPAWN);
			}
			else {
				auto superLeafNode = prefabDoc.child("Prefab").child("SuperLeaf");
				auto spawnedSuperLeaf = dynamic_cast<pSuperLeaf>(
					_game->Create(
						_scene,
						superLeafNode.attribute("actorID").as_int(),
						_name + superLeafNode.attribute("name").as_string(),
						superLeafNode.attribute("source").as_string(),
						_x, _y, _gx, _gy, _layer - 1
					)
					);
				spawnedSuperLeaf->SetAction(CSuperLeaf::EAction::SPAWN);
			}
		}
	} break;

	case CBlock::ESpawn::FIRE_FLOWER: {
		auto fireFlowerNode = prefabDoc.child("Prefab").child("FireFlower");
		auto spawnedFireFlower = dynamic_cast<pFireFlower>(
			_game->Create(
				_scene,
				fireFlowerNode.attribute("actorID").as_int(),
				_name + fireFlowerNode.attribute("name").as_string(),
				fireFlowerNode.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedFireFlower->SetAction(CFireFlower::EAction::SPAWN);
	} break;

	case CBlock::ESpawn::EXTRA_LIFE: {
		auto extraLifeMushroomNode = prefabDoc.child("Prefab").child("ExtraLifeMushroom");
		auto spawnedExtraLifeMushroom = dynamic_cast<pExtraLifeMushroom>(
			_game->Create(
				_scene,
				extraLifeMushroomNode.attribute("actorID").as_int(),
				_name + extraLifeMushroomNode.attribute("name").as_string(),
				extraLifeMushroomNode.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedExtraLifeMushroom->SetAction(CExtraLifeMushroom::EAction::SPAWN);
	} break;

	case CBlock::ESpawn::RELAY: {
		auto relayNode = prefabDoc.child("Prefab").child("Relay");
		auto spawnedRelay = dynamic_cast<pRelay>(
			_game->Create(
				_scene,
				relayNode.attribute("actorID").as_int(),
				_name + relayNode.attribute("name").as_string(),
				relayNode.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		//spawnedRelay->SetAction(CRelay::EAction::SPAWN);
	} break;
	
	}
}

void CBlock::HitBottom() {
	if (_action == EAction::IDLE) {
		_topBounce = true;
		SetNextAction(EAction::BOUNCE);
	}
}

void CBlock::HitSide(bool left) {
	if (_action == EAction::IDLE) {
		_topBounce = false;
		_leftBounce = left;
		SetNextAction(EAction::BOUNCE);
	}
}

#pragma endregion

#pragma region COLLISION

int CBlock::IsCollidable() {
	return 1;
}

int CBlock::IsBlocking() {
	return 1;
}

void CBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom) {
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CBlock::OnNoCollision(float elapsedMs) {
}

void CBlock::OnCollisionWith(pCollision collision) {
}

#pragma endregion