#pragma region INCLUDE
#include "Game.h"
#pragma endregion

Game::Game() {
	_application = new Application();
	_time = new Time();
	_graphics = new Graphics();
	_audio = new Audio();
	_input = new Input();
}

Game::~Game() {
	delete _input;
	_input = nullptr;

	delete _audio;
	_audio = nullptr;

	delete _graphics;
	_graphics = nullptr;

	delete _time;
	_time = nullptr;

	delete _application;
	_application = nullptr;
}

#pragma region Scene

Scene::Scene(unsigned int id, std::string source) {
	_id = id;
	_source = source;
}

void Scene::Add(unsigned int id) {
	if (std::find(_gameObjects.begin(), _gameObjects.end(), id) != _gameObjects.end()) {
		DebugOut(L"[Engine] GameObject with id %d already added to scene.\n", id);
		return;
	}

	_gameObjects.push_back(id);
}

void Scene::Remove(unsigned int id) {
	if (std::find(_gameObjects.begin(), _gameObjects.end(), id) == _gameObjects.end()) {
		DebugOut(L"[Engine] GameObject with id %d not existed in scene.\n", id);
		return;
	}

	_gameObjects.erase(
		std::remove(_gameObjects.begin(), _gameObjects.end(), id),
		_gameObjects.end()
	);
}

#pragma endregion

#pragma region Player

void Game::KeyState() {
	_previousButtonState = _currentButtonState;
}

void Game::OnKeyUp(int keyCode) {
	if (_currentButtonState.find(keyCode) != _currentButtonState.end()) {
		_currentButtonState[keyCode] = false;
	}
}

void Game::OnKeyDown(int keyCode) {
	if (_currentButtonState.find(keyCode) != _currentButtonState.end()) {
		_currentButtonState[keyCode] = true;
	}
}

void Game::BindKey(int keyCode) {
	_previousButtonState[keyCode] = false;
	_currentButtonState[keyCode] = false;
}

bool Game::IsKeyDown(int keyCode) {
	return _currentButtonState[keyCode];
}

bool Game::IsKeyPressed(int keyCode) {
	return
		!_previousButtonState[keyCode]
		&& _currentButtonState[keyCode];
}

bool Game::IsKeyReleased(int keyCode) {
	return
		_previousButtonState[keyCode]
		&& !_currentButtonState[keyCode];
}

#pragma endregion

#pragma region Scenes Database

void Game::AddScene(unsigned int id, std::string source) {
	if (_scenes.find(id) != _scenes.end()) {
		DebugOut(L"[Engine] Scene id already used: %d - %s.\n", id, source);
		return;
	}

	_scenes[id] = new Scene(id, source);
}

void Game::SceneLoading() {
	if (!_sceneLoading) {
		return;
	}

	for (auto& scene : _scenes) {
		if (!scene.second->_load) {
			continue;
		}

		if (scene.second->_play) {
			LoadScene(scene.second);
		}
		else {
			UnloadScene(scene.second);
		}
	}

	_sceneLoading = false;
}

void Game::LoadScene(pScene scene) {
	scene->_load = false;

	// call game to create game object
	// add created game object to scene
}

void Game::UnloadScene(pScene scene) {
	scene->_load = false;

	// call game object destroy function
	// remove destroyed game object from scene
	// all destroyed game objects will be purge
}

void Game::PlayScene(unsigned int id) {
	if (_scenes.find(id) == _scenes.end()) {
		DebugOut(L"[Engine] Scene id non existed: %d.\n", id);
		return;
	}

	_sceneLoading = true;
	_scenes[id]->_load = true;
	_scenes[id]->_play = true;
}

void Game::StopScene(unsigned int id) {
	if (_scenes.find(id) == _scenes.end()) {
		DebugOut(L"[Engine] Scene id non exist: %d.\n", id);
		return;
	}

	_sceneLoading = true;
	_scenes[id]->_load = true;
	_scenes[id]->_play = false;
}

#pragma endregion









void Game::Run(
	HINSTANCE hInstance,
	std::string sourcePath
) {
	if (!Load(hInstance, sourcePath)) {
		Shutdown();
		return;
	}

	_time->StartClock();
	float msPerFrame = 1000.0f / _frameRate;
	float elapsedMs = 0.0f;

	bool done = false;
	while (!done) {
		SceneLoading();
		_time->TickClock();
		elapsedMs += _time->GetElapsedMs();

		if (elapsedMs >= msPerFrame) {
			_input->ProcessKeyboard();
			Update(elapsedMs);
			Render();
			elapsedMs = 0.0f;
		}
		else {
			Sleep(DWORD(msPerFrame - elapsedMs));
		}

		done = _application->HandleMessage();
	}

	Shutdown();
}

bool Game::Load(
	HINSTANCE hInstance,
	std::string sourcePath
) {
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
	if (!result) {
		return false;
	}

	result = _graphics->Initialize(
		_application->GetWindow()
	);
	if (!result) {
		return false;
	}

	result = _audio->Initialize(
		_application->GetWindow()
	);
	if (!result) {
		return false;
	}

	result = _input->Initialize(
		_application->GetInstance(),
		_application->GetWindow(),
		this
	);
	if (!result) {
		return false;
	}

	for (auto texture = gameData.child("GameData").child("Texture");
		texture;
		texture = texture.next_sibling("Texture")) {
		_graphics->Load(
			texture.attribute("id").as_uint(),
			std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
				texture.attribute("source").as_string()
			)
		);
	}

	for (auto sound = gameData.child("GameData").child("Sound");
		sound;
		sound = sound.next_sibling("Sound")) {
		_audio->Load(
			sound.attribute("id").as_uint(),
			sound.attribute("source").as_string()
		);
	}

	for (auto scene = gameData.child("GameData").child("Scene");
		scene;
		scene = scene.next_sibling("Scene")) {
		AddScene(
			scene.attribute("id").as_uint(),
			scene.attribute("source").as_string()
		);
	}

	PlayScene(
		gameSettings.attribute("startScene").as_uint()
	);

	return true;
}

void Game::Update(float elapsedMs) {
}

void Game::Render() {
}

void Game::Shutdown() {

	for (auto& scene : _scenes) {
		delete scene.second;
		scene.second = nullptr;
	}
	_scenes.clear();

	_input->Shutdown();
	_audio->Shutdown();
	_graphics->Shutdown();
}



