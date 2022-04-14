#pragma once
#ifndef __GAME_MASTER_H__
#define __GAME_MASTER_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
#pragma endregion

class CGameMaster : public CGameObject {
public:
	CGameMaster(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, scene, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CGameMaster* pGameMaster;

#endif // !__GAME_MASTER_H__
