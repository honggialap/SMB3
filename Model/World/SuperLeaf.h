#pragma once
#ifndef __SUPER_LEAF_H__
#define __SUPER_LEAF_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX					999901
#define SPR_SUPER_LEAF_LEFT			100101
#define SPR_SUPER_LEAF_RIGHT		100102
#pragma endregion

class CSuperLeaf : public CGameObject {
public:
	CSuperLeaf(
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

	/* Move */
	float DROP_SPEED = 0;
	float MOVE_SPEED = 0;
	float MOVE_LIMIT = 0;
	float SPAWN_SPEED = 0;
	float SPAWN_LIMIT = 0;
	float _moveOrigin = 0;
	float _moveLimit = 0;
	float _spawnLimit = 0;
	std::string _targetName;
	bool _left = false;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		SPAWN,
		MOVE,
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
	void Move(float elapsedMs);
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
typedef CSuperLeaf* pSuperLeaf;

#endif // !__SUPER_LEAF_H__
