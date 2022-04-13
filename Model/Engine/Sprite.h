#pragma once
#ifndef __SPRITE_H__
#define __SPRITE_H__

#pragma region FORWARD DECLARATION
class CGameObject;
typedef CGameObject* pGameObject;
#pragma endregion

#pragma region INCLUDE
#include "Graphics.h"
#pragma endregion

class CSprite {
private:
	pGameObject _gameObject;

	int _left = 0;
	int _top = 0;
	int _width = 0;
	int _height = 0;
	int _offsetX = 0;
	int	_offsetY = 0;

	pTexture _texture = nullptr;
	D3DX10_SPRITE _sprite;
	D3DXMATRIX _matScaling;

public:
	CSprite(pGameObject gameObject, unsigned int left, unsigned int top, unsigned int width, unsigned int height, int offsetX, int offsetY, pTexture texture);
	~CSprite();

public:
	void Render(float x, float y, bool isUI = false);
};
typedef CSprite* pSprite;
#endif // !__SPRITE_H__
