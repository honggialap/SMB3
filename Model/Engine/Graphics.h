#pragma once
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#pragma region INCLUDE
#include <d3d10.h>
#include <D3DX10.h>
#include <unordered_map>
#pragma endregion

struct CTexture {
	ID3D10Texture2D* _texture = nullptr;
	ID3D10ShaderResourceView* _rsview = nullptr;
	int _width = 0;
	int _height = 0;

	~CTexture();
	CTexture(ID3D10Texture2D* texture, ID3D10ShaderResourceView* rsview);
};
typedef CTexture* pTexture;

class CGraphics {
private:
	ID3D10Device* _device = NULL;
	IDXGISwapChain* _swapChain = NULL;
	ID3D10RenderTargetView* _renderTargetView = NULL;
	ID3D10BlendState* _blendStateAlpha = NULL;
	ID3DX10Sprite* _spriteHandler = NULL;
	ID3D10SamplerState* _pointSamplerState = NULL;

	unsigned int _backBufferWidth = 0;
	unsigned int _backBufferHeight = 0;

	std::unordered_map<unsigned int, pTexture> _textures;

public:
	ID3D10Device* GetDevice() { return _device; }
	IDXGISwapChain* GetSwapChain() { return _swapChain; }
	ID3D10RenderTargetView* GetRenderTargetView() { return _renderTargetView; }
	ID3DX10Sprite* GetSpriteHandler() { return _spriteHandler; }
	ID3D10BlendState* GetBlendStateAlpha() { return _blendStateAlpha; }

	unsigned int GetBackBufferWidth() { return _backBufferWidth; }
	unsigned int GetBackBufferHeight() { return _backBufferHeight; }

	bool CreateSwapChain(HWND hWnd);
	bool CreateRenderTargetView();
	bool CreateSpriteHandler();
	bool CreateBlendState();

	bool Initialize(HWND hWnd);
	void Shutdown();

	pTexture LoadTextureFromFile(std::wstring source);
	void LoadTexture(unsigned int ID, std::wstring source);
	pTexture GetTexture(unsigned int ID);
};
typedef CGraphics* pGraphics;

#endif // !__GRAPHICS_H__
