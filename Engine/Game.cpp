#pragma region INCLUDE
#include "Game.h"
#include "Ultilities.h"
#pragma endregion

namespace Engine
{

	Game::Game()
	{
		_application = new Application();
		_time = new Time();
		_graphics = new Graphics();
		_textures = new Textures(_graphics);
		_audio = new Audio();
		_sounds = new Sounds(_audio);
		_input = new Input();
		_player = new Player();
		_grid = new Grid();
		_scenes = new Scenes();
		_gameObjects = new GameObjects();
	}

	Game::~Game()
	{
		delete _gameObjects;
		_gameObjects = nullptr;
	
		delete _scenes;
		_scenes = nullptr;

		delete _grid;
		_grid = nullptr;
		
		delete _player;
		_player = nullptr;

		delete _input;
		_input = nullptr;

		delete _sounds;
		_sounds = nullptr;

		delete _audio;
		_audio = nullptr;

		delete _textures;
		_textures = nullptr;

		delete _graphics;
		_graphics = nullptr;

		delete _time;
		_time = nullptr;

		delete _application;
		_application = nullptr;
	}

	void Game::Run(
		HINSTANCE hInstance,
		std::string sourcePath
	)
	{
		if (!Load(hInstance, sourcePath))
		{
			Shutdown();
			return;
		}

		_time->StartClock();
		float msPerFrame = 1000.0f / _frameRate;
		float elapsedMs = 0.0f;

		bool done = false;
		while (!done)
		{
			_time->TickClock();
			elapsedMs += _time->GetElapsedMs();

			if (elapsedMs >= msPerFrame)
			{
				_input->ProcessKeyboard();
				Update(elapsedMs);
				Render();
				elapsedMs = 0.0f;
			}
			else
			{
				Sleep(DWORD(msPerFrame - elapsedMs));
			}

			done = _application->HandleMessage();
		}

		Shutdown();
	}

	bool Game::Load(
		HINSTANCE hInstance,
		std::string sourcePath
	)
	{
		bool result;

		pugi::xml_document gameData;
		gameData.load_file(sourcePath.c_str());

		auto gameSettings = gameData.child("GameData").child("GameSettings");
		_frameRate = gameSettings.attribute("frameRate").as_float();

		result = _application->CreateGameWindow(
			hInstance,
			std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
				gameSettings.attribute("title").as_string()
			),
			gameSettings.attribute("resolutionWidth").as_uint(),
			gameSettings.attribute("resolutionHeight").as_uint()
		);
		if (!result)
		{
			return false;
		}

		result = _graphics->Initialize(
			_application->GetWindow()
		);
		if (!result)
		{
			return false;
		}

		result = _audio->Initialize(
			_application->GetWindow()
		);
		if (!result)
		{
			return false;
		}

		result = _input->Initialize(
			_application->GetInstance(),
			_application->GetWindow(),
			_player
		);
		if (!result)
		{
			return false;
		}

		for (
			auto texture = gameData.child("GameData").child("Texture");
			texture;
			texture = texture.next_sibling("Texture")
			)
		{
			_textures->Load(
				texture.attribute("id").as_uint(),
				std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
					texture.attribute("source").as_string()
				)
			);
		}		

		for (
			auto sound = gameData.child("GameData").child("Sound");
			sound;
			sound = sound.next_sibling("Sound")
			)
		{
			_sounds->Load(
				sound.attribute("id").as_uint(),
				sound.attribute("source").as_string()
			);
		}

		return true;
	}

	void Game::Update(float elapsedMs)
	{
	}

	void Game::Render()
	{
	}

	void Game::Shutdown()
	{
		_input->Shutdown();
		_sounds->Clean();
		_audio->Shutdown();
		_textures->Clean();
		_graphics->Shutdown();
	}

}
