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
	}

	Game::~Game()
	{
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
		_graphics->Shutdown();
	}

}
