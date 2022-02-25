#pragma region INCLUDE
#include "Game.h"
#pragma endregion

namespace Engine
{

	Game::Game()
	{
		_application = new Application();
		_time = new Time();
	}

	Game::~Game()
	{
		delete _time;
		_time = nullptr;

		delete _application;
		_application = nullptr;
	}

	void Game::Run(HINSTANCE hInstance)
	{
		Load(hInstance);

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

	void Game::Load(HINSTANCE hInstance)
	{
		//temporary
		std::wstring windowTitle = L"Game test";
		unsigned int windowWidth = 800;
		unsigned int windowHeight = 600;
		_frameRate = 120.0f;
		//

		_application->CreateGameWindow(
			hInstance,
			windowTitle,
			windowWidth,
			windowHeight
		);
	}

	void Game::Update(float elapsedMs)
	{
	}

	void Game::Render()
	{
	}

	void Game::Shutdown()
	{
	}

}
