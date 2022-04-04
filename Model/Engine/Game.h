#pragma once
#ifndef __GAME_H__
#define	__GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"
#include "Ultilities.h"
#include "GameObject.h"
#pragma endregion

struct Scene {
	unsigned int _id;
	std::string _source;
	std::vector<unsigned int> _gameObjects;
	bool _play = false;
	bool _load = false;

	Scene(unsigned int id, std::string source);

	void Add(unsigned int id);
	void Remove(unsigned int id);
};
typedef Scene* pScene;

class Game : public KeyHandler {
public:
	Game();
	~Game();

#pragma region Framework
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;
	pGraphics _graphics = nullptr;
	pAudio _audio = nullptr;
	pInput _input = nullptr;

public:
	pApplication GetApplication() { return _application; }
	pTime GetTime() { return _time; }
	pGraphics GetGraphics() { return _graphics; }
	pAudio GetAudio() { return _audio; }
	pInput GetInput() { return _input; }
#pragma endregion

#pragma region Player
protected:
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
#pragma endregion

#pragma region Scenes database
protected:
	std::map<unsigned int, pScene> _scenes;
	bool _sceneLoading = false;

protected:
	void AddScene(unsigned int id, std::string source);
	void SceneLoading();
	void LoadScene(pScene scene);
	void UnloadScene(pScene scene);

public:
	void PlayScene(unsigned int id);
	void StopScene(unsigned int id);
#pragma endregion

#pragma region Game Object

#pragma endregion

#pragma region Game loop
protected:
	float _frameRate = 0.0f;

protected:
	void Update(float elapsedMs);
	void Render();
	void Shutdown();

public:
	void Run(HINSTANCE hInstance, std::string sourcePath);
	bool Load(HINSTANCE hInstance, std::string sourcePath);
#pragma endregion
};
typedef Game* pGame;

#endif // !__GAME_H__
