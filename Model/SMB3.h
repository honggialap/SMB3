#pragma once
#ifndef __SMB3_H__
#define __SMB3_H__

#pragma region INCLUDE
#include "Engine/Game.h"
#pragma endregion

#pragma region ACTOR
#define ACT_GAME_MASTER			10101
#define ACT_MENU				10201
#define ACT_WORLD_MAP			10301
#define ACT_WORLD				10401

#define ACT_MARIO				20101
#define ACT_MARIO_TAIL			20102
#define ACT_MARIO_FIREBALL		20103
#define ACT_GOOMBA				20201
#define ACT_MICRO_GOOMBA		20202
#define ACT_KOOPA				20301
#define ACT_KOOPA_SENSOR		20302
#define ACT_PLANT				20401
#define ACT_PLANT_SENSOR		20402
#define ACT_PLANT_FIREBALL		20403

#define ACT_COIN				30101
#define ACT_SUPER_MUSHROOM		30201
#define ACT_FIRE_FLOWER			30301
#define ACT_SUPER_LEAF			30401
#define ACT_EXTRA_LIFE_MUSHROOM	30501

#define ACT_PLATFORM			40101
#define ACT_BLOCK				40201
#define ACT_BRICK				40301
#define ACT_PIPE				40401
#define ACT_TRANSPORT_PIPE		40402
#define ACT_RELAY				40501
#define ACT_SPAWN_ZONE			40601
#define ACT_DEAD_ZONE			40701
#define ACT_GOAL				40801

#define ACT_BACKGROUND			50101
#pragma endregion

class CSMB3 : public CGame {
public:
	pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY, int gridX, int gridY,
		unsigned int layer
	);
};
typedef CSMB3* pSMB3;

#endif // !__SMB3_H__


