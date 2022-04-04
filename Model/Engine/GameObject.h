#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#pragma region INCLUDE
#include "Component/Sprite.h"
#include "Component/Animation.h"
#include "Component/Collision.h"
#include "Component/AudioSource.h"
#pragma endregion

#pragma region FORWARD DECLARATION
class Game;
typedef Game* pGame;
#pragma endregion

class GameObject {

};
typedef GameObject* pGameObject;

#endif // !__GAME_OBJECT_H__
