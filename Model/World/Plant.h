#pragma once
#ifndef __PLANT_H__
#define __PLANT_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#include "PlantSensor.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX						999901

#define SPR_PLANT_UP1					110101
#define SPR_PLANT_UP2					110102
#define SPR_PLANT_UP_LEFT_DOWN1			120101
#define SPR_PLANT_UP_LEFT_DOWN2			120102
#define SPR_PLANT_UP_LEFT_UP1			130101
#define SPR_PLANT_UP_LEFT_UP2			130102
#define SPR_PLANT_UP_RIGHT_DOWN1		140101
#define SPR_PLANT_UP_RIGHT_DOWN2		140102
#define SPR_PLANT_UP_RIGHT_UP1			150101
#define SPR_PLANT_UP_RIGHT_UP2			150102

#define SPR_PLANT_DOWN1					210101
#define SPR_PLANT_DOWN2					210102
#define SPR_PLANT_DOWN_LEFT_DOWN1		220101
#define SPR_PLANT_DOWN_LEFT_DOWN2		220102
#define SPR_PLANT_DOWN_LEFT_UP1			230101
#define SPR_PLANT_DOWN_LEFT_UP2			230102
#define SPR_PLANT_DOWN_RIGHT_DOWN1		240101
#define SPR_PLANT_DOWN_RIGHT_DOWN2		240102
#define SPR_PLANT_DOWN_RIGHT_UP1		250101
#define SPR_PLANT_DOWN_RIGHT_UP2		250102

#define SPR_RED_PLANT_UP1				310101
#define SPR_RED_PLANT_UP2				310102
#define SPR_RED_PLANT_UP_LEFT_DOWN1		320101
#define SPR_RED_PLANT_UP_LEFT_DOWN2		320102
#define SPR_RED_PLANT_UP_LEFT_UP1		330101
#define SPR_RED_PLANT_UP_LEFT_UP2		330102
#define SPR_RED_PLANT_UP_RIGHT_DOWN1	340101
#define SPR_RED_PLANT_UP_RIGHT_DOWN2	340102
#define SPR_RED_PLANT_UP_RIGHT_UP1		350101
#define SPR_RED_PLANT_UP_RIGHT_UP2		350102

#define SPR_RED_PLANT_DOWN1				410101
#define SPR_RED_PLANT_DOWN2				410102
#define SPR_RED_PLANT_DOWN_LEFT_DOWN1	420101
#define SPR_RED_PLANT_DOWN_LEFT_DOWN2	420102
#define SPR_RED_PLANT_DOWN_LEFT_UP1		430101
#define SPR_RED_PLANT_DOWN_LEFT_UP2		430102
#define SPR_RED_PLANT_DOWN_RIGHT_DOWN1	440101
#define SPR_RED_PLANT_DOWN_RIGHT_DOWN2	440102
#define SPR_RED_PLANT_DOWN_RIGHT_UP1	450101
#define SPR_RED_PLANT_DOWN_RIGHT_UP2	450102

/* ANIMATION ID */
#define ANI_PLANT_UP				1101
#define ANI_PLANT_DOWN				2101

#define ANI_RED_PLANT_UP			3101
#define ANI_RED_PLANT_DOWN			4101
#pragma endregion

class CPlant : public CGameObject {
public:
	CPlant(
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
	enum class EPlant {
		PIRANHA,
		VENUS
	};
	EPlant _plant = EPlant::PIRANHA;
	enum class EType {
		GREEN,
		RED
	};
	EType _type = EType::GREEN;
	bool _growUp = false;

	/* Sensor */
	pPlantSensor _sensor = nullptr;

	/* Grow */
	float GROW_LIMIT = 0;
	float GROW_SPEED = 0;
	float STAND_INTERVAL = 0;
	float HIDE_INTERVAL = 0;
	float _growLimit = 0;
	float _growOrigin = 0;
	float _standInterval = 0;
	float _hideInterval = 0;

	/* Shoot */
	float SHOOT_OFFSETY = 0;
	float SHOOT_INTERVAL = 0;
	float SHOOT_HORIZONTAL_SPEED = 0;
	float SHOOT_VERTICAL_SPEED = 0;
	float _shootInterval = 0;
	bool _left = false;
	bool _up = false;
	std::string _targetName;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		HIDE,
		GROW,
		STAND,
		BURROW,
		SHOOT,
		DIE
	};
	EAction _action = EAction::HIDE;
	EAction _nextAction = EAction::HIDE;

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
	void Hide(float elapsedMs);
	void Grow(float elapsedMs);
	void Stand(float elapsedMs);
	void Burrow(float elapsedMs);
	void Shoot(float elapsedMs);
	void Die(float elapsedMs);

	void UpdateSensor(float elapsedMs);
	void AcquireTarget();
	void ShootFireball();
	void Hit();

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CPlant* pPlant;

#endif // !__PLANT_H__
