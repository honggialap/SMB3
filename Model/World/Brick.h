#pragma once
#ifndef __BRICK_H__
#define __BRICK_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
/* SPRITE ID */
#define SPR_BBOX				999901

#define SPR_BRICK1				100101
#define SPR_BRICK2				100102
#define SPR_BRICK3				100103
#define SPR_BRICK4				100104

/* ANIMATION ID */
#define ANI_BRICK				1001
#pragma endregion

class CBrick : public CGameObject {
public:
	CBrick(
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

	/* Bounce */
	float BOUNCE_SPEED = 0;
	float BOUNCE_LIMIT = 0;
	float _bounceLimit = 0;
	float _bounceOrigin = 0;
	bool _return = false;
	float _renderX = 0;
	float _renderY = 0;
	std::string _targetName;

#pragma endregion

#pragma region STATE MACHINE

	enum class EAction {
		IDLE,
		BOUNCE,
		BROKE
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
	void Broke(float elapsedMs);

	void HitBottom();
	void Brake();

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CBrick* pBrick;

#endif // !__BRICK_H__
