#pragma once
#ifndef __GOOMBA_H__
#define __GOOMBA_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX					999901

#define SPR_GOOMBA_WALK1_UP			110101
#define SPR_GOOMBA_WALK2_UP			110102
#define SPR_GOOMBA_WALK1_DOWN		120101
#define SPR_GOOMBA_WALK2_DOWN		120102
#define SPR_GOOMBA_DIE				110201

#define SPR_RED_GOOMBA_WALK1_UP		210101
#define SPR_RED_GOOMBA_WALK2_UP		210102
#define SPR_RED_GOOMBA_WALK1_DOWN	220101
#define SPR_RED_GOOMBA_WALK2_DOWN	220102
#define SPR_RED_GOOMBA_DIE			210201

#define SPR_GOOMBA_WING1			310101
#define SPR_GOOMBA_WING2			310102

/* ANIMATION ID */
#define ANI_GOOMBA_WALK_UP			1101
#define ANI_GOOMBA_WALK_DOWN		1201

#define ANI_RED_GOOMBA_WALK_UP		2101
#define ANI_RED_GOOMBA_WALK_DOWN	2201

#define ANI_GOOMBA_WING_FLAP_SLOW	3101
#define ANI_GOOMBA_WING_FLAP_FAST	3102

#pragma endregion

class CGoomba : public CGameObject {
public:
	CGoomba(
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
		BROWN,
		RED
	};
	EType _type = EType::BROWN;
	bool _wing = false;
	bool _spawn = false;

	/* Target */
	std::string _targetName;
	float _targetX = 0;
	float _targetY = 0;

	/* Gravity */
	float GRAVITY = 0;
	bool _ground = false;

	/* Move */
	float WALK_SPEED = 0;
	bool _left = false;

	/* Jump */
	float SMALL_JUMP_FORCE = 0;
	float LARGE_JUMP_FORCE = 0;
	float JUMP_INTERVAL = 0;
	float _jumpInterval = 0;

	/* Fly */
	float TAKE_OFF_SPEED = 0;
	float TARGET_FLY_OFFSET = 0;
	float FLY_TIMEOUT = 0;
	float CHANGE_DIRECTION_COUNTDOWN = 0;
	float _flyTimeout = 0;
	float _changeDirectionCountdown = 0;

	/* Hit */
	float HORIZONTAL_DEFLECT_FORCE = 0;
	float VERTICAL_DEFLECT_FORCE = 0;
	float DECAY_TIMEOUT = 0;
	float _decayTimeout = 0;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		MOVE,
		SMALL_JUMP,
		LARGE_JUMP,
		TAKEOFF,
		FLY,
		LAND,
		DIE,
		THROWN,
		DEADZONED
	};
	EAction _action = EAction::MOVE;
	EAction _nextAction = EAction::MOVE;

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
	void Move(float elapsedMs);
	void SmallJump(float elapsedMs);
	void LargeJump(float elapsedMs);
	void TakeOff(float elapsedMs);
	void Fly(float elapsedMs);
	void Land(float elapsedMs);
	void Die(float elapsedMs);
	void Thrown(float elapsedMs);
	void DeadZoned(float elapsedMs);

	void UpdateGravity(float elapsedMs);

	void HitTop();
	void HitSide(bool left);
	void AcquireTarget();
	void SpawnMicroGoomba();

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

	void OnCollisionWithPlatform(pCollision collision);

#pragma endregion

};
typedef CGoomba* pGoomba;

#endif // !__GOOMBA_H__
