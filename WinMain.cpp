#pragma region INCLUDE
#include "Model/SMB3.h"
#pragma endregion

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR lpCmdLine,
	_In_ int nCmdShow
) {
	auto game = new CSMB3();

	game->Run(hInstance, "Data/GameData.xml");

	delete game;
	game = nullptr;
}