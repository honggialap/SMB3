#include "Model/SMB3.h"

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
)
{
	auto game = new Engine::Game();

	game->Run(hInstance, "Data/GameData.xml");

	delete game;
	game = nullptr;
}