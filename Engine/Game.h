#pragma once
#ifndef __GAME_H__
#define	__GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#pragma endregion

namespace Engine
{

	class Game
	{
	private:
		pApplication _application = nullptr;
		pTime _time = nullptr;

		float _frameRate = 0.0f;

	public:
		pApplication GetApplication() { return _application; }
		pTime GetTime() { return _time; }

		Game();
		~Game();

		void Run(
			HINSTANCE hInstance,
			std::string sourcePath
		);
		void Load(
			HINSTANCE hInstance,
			std::string sourcePath
		);
		void Update(float elapsedMs);
		void Render();
		void Shutdown();
	};

}

#endif // !__GAME_H__
