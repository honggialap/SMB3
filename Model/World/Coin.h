#pragma once
#ifndef __COIN_H__
#define __COIN_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_COIN1				100101
#define SPR_COIN2				100102
#define SPR_COIN3				100103
#define SPR_COIN4				100104
#define SPR_COIN5				100105
#define SPR_COIN6				100106

/* ANIMATION ID */
#define ANI_COIN_SPIN_SLOW		1001
#define ANI_COIN_SPIN_FAST		1002

#pragma endregion

class CCoin : public CGameObject {
public:
	CCoin(
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

	/* Spin */
	float GRAVITY = 0;
	float BOUNCE_FORCE = 0;
	float DECAY_INTERVAL = 0;
	float _decayInterval = 0;
	bool _spin = false;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		IDLE,
		CONSUMED
	};
	EAction _action = EAction::IDLE;
	EAction _nextAction = EAction::IDLE;

	enum class EActionStage	{
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
typedef CCoin* pCoin;

#endif // !__COIN_H__