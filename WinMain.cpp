#include "Engine/Application.h"
#include "Engine/Time.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto application = new Engine::Application();
	auto time = new Engine::Time();

	std::wstring windowTitle = L"Time test";
	unsigned int windowWidth = 800;
	unsigned int windowHeight = 600;

	application->CreateGameWindow(
		hInstance, 
		windowTitle,
		windowWidth, 
		windowHeight
	);
	
	time->StartClock();

	float elapsedMs = 0.0f;
	float lifeTimeSpan = 10000.0f;
	while(!application->HandleMessage())
	{
		time->TickClock();

		elapsedMs += time->GetElapsedMs();
		if (elapsedMs > lifeTimeSpan)
		{
			application->Exit();
		}
	}

	delete time;
	time = nullptr;

	delete application;
	application = nullptr;
}