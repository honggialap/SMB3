#pragma once
#ifndef __FIRE_FLOWER_H__
#define __FIRE_FLOWER_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX			999901
#define SPR_FIRE_FLOWER		100101
#pragma endregion

class CFireFlower : public CGameObject {
public:
	CFireFlower(
		pGame game, pScene scene,
		unsigned int ID, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, scene, ID, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

#pragma region LOGIC

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Spawn */
	float SPAWN_SPEED = 0;
	float SPAWN_LIMIT = 0;
	float _spawnLimit = 0;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		SPAWN,
		IDLE,
		CONSUMED
	};
	EAction _action = EAction::SPAWN;
	EAction _nextAction = EAction::SPAWN;

	enum class EActionStage {
		ENTRY,
		PROGRESS,
		EXIT
	};
	EActionStage _actionStage = EActionStage::ENTRY;

	void SetAction(EAction action) {
		_action = action;
		_actionStage = EActionStage::ENTRY;
	}
	void SetNextAction(EAction action) {
		_nextAction = action;
		_actionStage = EActionStage::EXIT;
	}
	void NextAction() {
		_action = _nextAction;
		_actionStage = EActionStage::ENTRY;
	}

	void HandleAction(float elapsedMs);
	void Spawn(float elapsedMs);
	void Idle(float elapsedMs);
	void Consumed(float elapsedMs);

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CFireFlower* pFireFlower;

#endif // !__FIRE_FLOWER_H__
