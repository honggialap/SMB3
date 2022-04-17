#pragma once
#ifndef __PLANT_FIREBALL_H__
#define __PLANT_FIREBALL_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
#define SPR_BBOX									999901

#define SPR_PLANT_FIREBALL1_LEFT					110101
#define SPR_PLANT_FIREBALL2_LEFT					110102
#define SPR_PLANT_FIREBALL3_LEFT					110103
#define SPR_PLANT_FIREBALL4_LEFT					110104

#define SPR_PLANT_FIREBALL1_RIGHT					120101
#define SPR_PLANT_FIREBALL2_RIGHT					120102
#define SPR_PLANT_FIREBALL3_RIGHT					120103
#define SPR_PLANT_FIREBALL4_RIGHT					120104

/* ANIMATION ID */
#define ANI_PLANT_FIREBALL_LEFT						1101
#define ANI_PLANT_FIREBALL_RIGHT					1201
#pragma endregion

class CPlantFireball : public CGameObject {
public:
	CPlantFireball(
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

	/* Lifespan */
	float DECAY_INTERVAL = 0;
	float _decayInterval = 0;

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

#pragma endregion
};
typedef CPlantFireball* pPlantFireball;

#endif // !__PLANT_FIREBALL_H__
