#pragma once
#ifndef __GAME_H__
#define	__GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Texture.h"
#include "Audio.h"
#include "Sound.h"
#include "Input.h"
#include "Player.h"
#pragma endregion

namespace Engine
{

	class Game
	{
	private:
		pApplication _application = nullptr;
		pTime _time = nullptr;
		pGraphics _graphics = nullptr;
		pTextures _textures = nullptr;
		pAudio _audio = nullptr;
		pSounds _sounds = nullptr;
		pInput _input = nullptr;
		pPlayer _player = nullptr;

		float _frameRate = 0.0f;

	public:
		pApplication GetApplication() { return _application; }
		pTime GetTime() { return _time; }
		pGraphics GetGraphics() { return _graphics; }
		pTextures GetTextures() { return _textures; }
		pAudio GetAudio() { return _audio; }
		pSounds GetSounds() { return _sounds; }
		pInput GetInput() { return _input; }
		pPlayer GetPlayer() { return _player; }

		Game();
		~Game();

		void Run(
			HINSTANCE hInstance,
			std::string sourcePath
		);
		bool Load(
			HINSTANCE hInstance,
			std::string sourcePath
		);
		void Update(float elapsedMs);
		void Render();
		void Shutdown();
	};
	typedef Game* pGame;

}

#endif // !__GAME_H__
