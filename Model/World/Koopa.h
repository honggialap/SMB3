#pragma once
#ifndef __KOOPA_H__
#define __KOOPA_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#include "KoopaSensor.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX									999901

#define SPR_KOOPA_WALK1_UP_LEFT						110101
#define SPR_KOOPA_WALK2_UP_LEFT						110102
#define SPR_KOOPA_WALK1_UP_RIGHT					120101
#define SPR_KOOPA_WALK2_UP_RIGHT					120102

#define SPR_KOOPA_WALK1_DOWN_LEFT					130101
#define SPR_KOOPA_WALK2_DOWN_LEFT					130102
#define SPR_KOOPA_WALK1_DOWN_RIGHT					140101
#define SPR_KOOPA_WALK2_DOWN_RIGHT					140102

#define SPR_KOOPA_SHELL1_UP							150101
#define SPR_KOOPA_SHELL2_UP							150102
#define SPR_KOOPA_SHELL3_UP							150103
#define SPR_KOOPA_SHELL4_UP							150104
#define SPR_KOOPA_RETRACT_UP						150201

#define SPR_KOOPA_SHELL1_DOWN						160101
#define SPR_KOOPA_SHELL2_DOWN						160102
#define SPR_KOOPA_SHELL3_DOWN						160103
#define SPR_KOOPA_SHELL4_DOWN						160104
#define SPR_KOOPA_RETRECT_DOWN						160201

#define SPR_RED_KOOPA_WALK1_UP_LEFT					210101
#define SPR_RED_KOOPA_WALK2_UP_LEFT					210102
#define SPR_RED_KOOPA_WALK1_UP_RIGHT				220101
#define SPR_RED_KOOPA_WALK2_UP_RIGHT				220102

#define SPR_RED_KOOPA_WALK1_DOWN_LEFT				230101
#define SPR_RED_KOOPA_WALK2_DOWN_LEFT				230102
#define SPR_RED_KOOPA_WALK1_DOWN_RIGHT				240101
#define SPR_RED_KOOPA_WALK2_DOWN_RIGHT				240102

#define SPR_RED_KOOPA_SHELL1_UP						250101
#define SPR_RED_KOOPA_SHELL2_UP						250102
#define SPR_RED_KOOPA_SHELL3_UP						250103
#define SPR_RED_KOOPA_SHELL4_UP						250104
#define SPR_RED_KOOPA_RETRACT_UP					250201

#define SPR_RED_KOOPA_SHELL1_DOWN					260101
#define SPR_RED_KOOPA_SHELL2_DOWN					260102
#define SPR_RED_KOOPA_SHELL3_DOWN					260103
#define SPR_RED_KOOPA_SHELL4_DOWN					260104
#define SPR_RED_KOOPA_SHELL5_DOWN					260201

#define SPR_KOOPA_WING1_LEFT						310101
#define SPR_KOOPA_WING2_LEFT						310102

#define SPR_KOOPA_WING1_RIGHT						320101
#define SPR_KOOPA_WING2_RIGHT						320102

/* ANIMATION ID */
#define ANI_KOOPA_WALK_UP_LEFT						1101
#define ANI_KOOPA_WALK_UP_RIGHT						1201
#define ANI_KOOPA_WALK_DOWN_LEFT					1301
#define ANI_KOOPA_WALK_DOWN_RIGHT					1401

#define ANI_KOOPA_SHELL_SPIN_UP_LEFT				1501
#define ANI_KOOPA_SHELL_SPIN_UP_RIGHT				1502
#define ANI_KOOPA_SHELL_SPIN_DOWN_LEFT				1601
#define ANI_KOOPA_SHELL_SPIN_DOWN_RIGHT				1602

#define ANI_KOOPA_SHELL_RETRACT_UP					1701
#define ANI_KOOPA_SHELL_RETRACT_DOWN				1702

#define ANI_RED_KOOPA_WALK_UP_LEFT					2101
#define ANI_RED_KOOPA_WALK_UP_RIGHT					2201
#define ANI_RED_KOOPA_WALK_DOWN_LEFT				2301
#define ANI_RED_KOOPA_WALK_DOWN_RIGHT				2401

#define ANI_RED_KOOPA_SHELL_SPIN_UP_LEFT			2501
#define ANI_RED_KOOPA_SHELL_SPIN_UP_RIGHT			2502
#define ANI_RED_KOOPA_SHELL_SPIN_DOWN_LEFT			2601
#define ANI_RED_KOOPA_SHELL_SPIN_DOWN_RIGHT			2602

#define ANI_RED_KOOPA_SHELL_RETRACT_UP				2701
#define ANI_RED_KOOPA_SHELL_RETRACT_DOWN			2702

#define ANI_KOOPA_WING_FLAP_SLOW_LEFT				3101
#define ANI_KOOPA_WING_FLAP_FAST_LEFT				3102

#define ANI_KOOPA_WING_FLAP_SLOW_RIGHT				3201
#define ANI_KOOPA_WING_FLAP_FAST_RIGHT				3202
#pragma endregion

class CKoopa : public CGameObject {
public:
	CKoopa(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, scene, id, name, source, x, y, gx, gy, layer) {};

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
		GREEN,
		RED
	};
	EType _type = EType::GREEN;
	bool _wing = false;

	/* Sensor */
	float SENSOR_OFFSETX = 0;
	float SENSOR_OFFSETY = 0;
	pKoopaSensor _sensor = nullptr;

	/* Gravity */
	float GRAVITY = 0;
	bool _ground = false;

	/* Move */
	float WALK_SPEED = 0;
	bool _left = false;
	bool _up = false;

	/* Jump */
	float JUMP_FORCE = 0;

	/* fly */
	float FLY_LIMIT = 0;
	float FLY_ACCELERATION = 0;
	float FLY_MAX_SPEED = 0;
	float _flyOrigin = 0;
	float _flyLimit = 0;
	bool _flyUp = false;

	/* Hit */
	float HORIZONTAL_DEFLECT_FORCE = 0;
	float VERTICAL_DEFLECT_FORCE = 0;
	float DECAY_TIMEOUT = 0;
	float _decayTimeout = 0;
	bool _hold = false;
	bool _kick = false;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		MOVE,
		JUMP,
		FLY,
		SHELL,
		RETRACT,
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
	void Jump(float elapsedMs);
	void Fly(float elapsedMs);
	void Shell(float elapsedMs);
	void Retract(float elapsedMs);
	void Thrown(float elapsedMs);
	void DeadZoned(float elapsedMs);

	void UpdateGravity(float elapsedMs);
	void UpdateSensor(float elapsedMs);

	void HitTop(bool left);
	void HitSide(bool left);
	void Kick(bool left);

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

	void OnCollisionWithGoomba(pCollision collision);

	void OnCollisionWithPlatform(pCollision collision);

#pragma endregion
};
typedef CKoopa* pKoopa;

#endif // !__KOOPA_H__
