#pragma once
#ifndef __BLOCK_H__
#define __BLOCK_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_QUESTION_BLOCK1		100101
#define SPR_QUESTION_BLOCK2		100102
#define SPR_QUESTION_BLOCK3		100103
#define SPR_QUESTION_BLOCK4		100104

#define SPR_BRICK_BLOCK1		100201
#define SPR_BRICK_BLOCK2		100202
#define SPR_BRICK_BLOCK3		100203
#define SPR_BRICK_BLOCK4		100204

#define SPR_EMPTY_BLOCK			100301

/* ANIMATION ID */
#define ANI_QUESTION_BLOCK		1001
#define ANI_BRICK_BLOCK			1002
#pragma endregion

class CBlock : public CGameObject {
public:
	CBlock(
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

	/* Type */
	enum class EType {
		QUESTION,
		BRICK
	};
	EType _type = EType::QUESTION;
	enum class ESpawn {
		COIN,
		POWER_UP,
		FIRE_FLOWER,
		EXTRA_LIFE,
		RELAY
	};
	ESpawn _spawn = ESpawn::COIN;
	int _spawnCount = 0;

	/* Bounce */
	float BOUNCE_SPEED = 0;
	float BOUNCE_LIMIT = 0;
	float _bounceLimit = 0;
	float _bounceOrigin = 0;
	bool _topBounce = false;
	bool _leftBounce = false;
	bool _return = false;
	float _renderX = 0;
	float _renderY = 0;
	std::string _targetName;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		IDLE,
		BOUNCE,
		EMPTY
	};
	EAction _action = EAction::IDLE;
	EAction _nextAction = EAction::IDLE;

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
	void Idle(float elapsedMs);
	void Bounce(float elapsedMs);
	void Empty(float elapsedMs);

	void Spawn();
	void HitBottom();
	void HitSide(bool left);

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CBlock* pBlock;

#endif // !__BLOCK_H__
