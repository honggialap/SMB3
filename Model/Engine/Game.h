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

	CScene(unsigned int sceneID, std::string _source);
	void Add(unsigned int gameObjectID);
	void Remove(unsigned int gameObjectID);
	void Clean();
};
typedef CScene* pScene;

class CGame : public IKeyHandler {
protected:
	pApplication _application = nullptr;
	pTime _time = nullptr;
	pGraphics _graphics = nullptr;
	pAudio _audio = nullptr;
	pInput _input = nullptr;

	std::map<int, bool> _currentButtonState;
	std::map<int, bool> _previousButtonState;

	float _cameraX = 0;
	float _cameraY = 0;
	float _cameraBuffer = 0;

	std::map<unsigned int, pScene> _scenes;
	bool _sceneLoading = false;

	unsigned int _nextGameObjectID = 0;
	std::unordered_map<unsigned int, pGameObject> _gameObjects;
	std::unordered_map<std::string, unsigned int> _dictionary;
	
	int _gridWidth = 0;
	int _gridHeight = 0;
	std::map<std::pair<int, int>, std::vector<unsigned int>> _grid;

	float _frameRate = 0.0f;
	std::vector<unsigned int> _updateQueue;
	std::vector<pGameObject> _renderQueue;

public:
	CGame();
	~CGame();

	pApplication GetApplication() { return _application; }
	pTime GetTime() { return _time; }
	pGraphics GetGraphics() { return _graphics; }
	pAudio GetAudio() { return _audio; }
	pInput GetInput() { return _input; }

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

	void AddScene(unsigned int sceneID, std::string source);
	void SceneLoading();

	void LoadScene(pScene scene);
	void UnloadScene(pScene scene);

	void PlayScene(unsigned int sceneID);
	void StopScene(unsigned int sceneID);

	virtual pGameObject Create(
		pScene scene,
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY,
		int gridX, int gridY,
		unsigned int layer
	) = 0;
	void Purge();

	void AddGameObject(pGameObject gameObject);
	pGameObject GetGameObject(unsigned int gameObjectID);
	pGameObject GetGameObject(std::string gameObjectName);
	std::vector<unsigned int> GetActives();

	void AddGrid(unsigned int gameObjectID);
	void RemoveGrid(unsigned int gameObjectID);
	void UpdateGrid(unsigned int gameObjectID);
	std::vector<pGameObject> GetLocal(unsigned int gameObjectID);

	void Run(HINSTANCE hInstance, std::string source);
	bool Load(HINSTANCE hInstance, std::string source);
	void Update(float elapsedMs);
	void Render();
	void Shutdown();
};
typedef CGame* pGame;

#endif // !__GAME_H__
