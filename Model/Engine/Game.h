#pragma once
#ifndef __GAME_H__
#define	__GAME_H__

#pragma region INCLUDE
#include "Application.h"
#include "Time.h"
#include "Graphics.h"
#include "Audio.h"
#include "Input.h"
#include "GameObject.h"
#include "Ultilities.h"
#pragma endregion

struct CScene {
	unsigned int _id;
	std::string _source;
	std::vector<unsigned int> _gameObjects;
	bool _play = false;
	bool _load = false;

	CScene(unsigned int id, std::string _source);
	void Add(unsigned int id);
	void Remove(unsigned int id);
	void Clean();
};
typedef CScene* pScene;

class CGame : public IKeyHandler {
public:
	CGame();
	~CGame();

	/* Framework */
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

	/* Player */
protected:
	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

	float _cameraX = 0;
	float _cameraY = 0;
	float _cameraBuffer = 0;

protected:
	virtual void KeyState();
	virtual void OnKeyUp(int keyCode);
	virtual void OnKeyDown(int keyCode);

	void BindKey(int keyCode);

public:
	bool IsKeyDown(int keyCode);
	bool IsKeyPressed(int keyCode);
	bool IsKeyReleased(int keyCode);

	float GetCameraBuffer() { return _cameraBuffer; }
	void GetCamera(float& x, float& y) { x = _cameraX; y = _cameraY; }
	void MoveCameraTo(float x, float y) { _cameraX = x; _cameraY = y; }
	void MoveCameraBy(float x, float y) { _cameraX += x; _cameraY += y; }

	/* Scenes Database */
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

	/* Game Objects Database */
protected:
	unsigned int nextGameObjectID = 0;
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;
	
	int _gridWidth = 0;
	int _gridHeight = 0;
	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;

protected:
	void AddGameObject(pGameObject gameObject);
	void Purge();

	void AddGrid(unsigned int gameObjectID);
	void RemoveGrid(unsigned int gameObjectID);
	void UpdateGrid(unsigned int gameObjectID);

public:
	virtual pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY,
		int gridX, int gridY,
		unsigned int layer
	) = 0;

	pGameObject GetGameObject(unsigned int id);
	pGameObject GetGameObject(std::string name);

	std::vector<unsigned int> GetActives();
	std::vector<pGameObject> GetLocal(unsigned int gameObjectID);

	/* Game loop */
protected:
	float _frameRate = 0.0f;
	std::vector<unsigned int> _updateQueue;
	std::vector<pGameObject> _renderQueue;

protected:
	bool Load(HINSTANCE hInstance, std::string sourcePath);
	void Update(float elapsedMs);
	void Render();
	void Shutdown();

public:
	void Run(HINSTANCE hInstance, std::string sourcePath);
};
typedef CGame* pGame;

#endif // !__GAME_H__
