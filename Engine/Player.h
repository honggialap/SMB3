#pragma once
#ifndef __PLAYER_H__
#define __PLAYER_H__

#pragma region INCLUDE
#include "Input.h"
#include <map>
#pragma endregion

namespace Engine
{

	class Player : public KeyHandler
	{
	private:
		std::map<int, bool> _currentButtonState;
		std::map<int, bool> _previousButtonState;

		float _cameraX = 0;
		float _cameraY = 0;
		float _cameraBuffer = 0;

	public:
		virtual void KeyState();
		virtual void OnKeyUp(int keyCode);
		virtual void OnKeyDown(int keyCode);

		void BindKey(int keyCode);
		bool IsKeyDown(int keyCode);
		bool IsKeyPressed(int keyCode);
		bool IsKeyReleased(int keyCode);

		float GetCameraBuffer() { return _cameraBuffer; }
		void GetCamera(float& x, float& y) { x = _cameraX; y = _cameraY; }
		void MoveCameraTo(float x, float y) { _cameraX = x; _cameraY = y; }
		void MoveCameraBy(float x, float y) { _cameraX += x; _cameraY += y; }
	};
	typedef Player* pPlayer;

}

#endif // !__PLAYER_H__
