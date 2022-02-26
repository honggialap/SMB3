#pragma once
#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#pragma region INCLUDE
#include <d3d10.h>
#include <D3DX10.h>
#pragma endregion

namespace Engine
{

	class Graphics
	{
	private:
		ID3D10Device* _device = NULL;
		IDXGISwapChain* _swapChain = NULL;
		ID3D10RenderTargetView* _renderTargetView = NULL;
		ID3D10BlendState* _blendStateAlpha = NULL;
		ID3DX10Sprite* _spriteHandler = NULL;
		ID3D10SamplerState* _pointSamplerState = NULL;

		unsigned int _backBufferWidth = 0;
		unsigned int _backBufferHeight = 0;

	public:
		ID3D10Device* GetDevice() { return _device; }
		IDXGISwapChain* GetSwapChain() { return _swapChain; }
		ID3D10RenderTargetView* GetRenderTargetView() { return _renderTargetView; }
		ID3DX10Sprite* GetSpriteHandler() { return _spriteHandler; }
		ID3D10BlendState* GetBlendStateAlpha() { return _blendStateAlpha; }

		unsigned int GetBackBufferWidth() { return _backBufferWidth; }
		unsigned int GetBackBufferHeight() { return _backBufferHeight; }

		bool Initialize(
			HWND hWnd
		);
		void Shutdown();

	private:
		bool CreateSwapChain(
			HWND hWnd
		);
		bool CreateRenderTargetView();
		bool CreateSpriteHandler();
		bool CreateBlendState();
	};
	typedef Graphics* pGraphics;

}

#endif // !__GRAPHICS_H__
