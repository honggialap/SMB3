#pragma region INCLUDE
#include "Game.h"
#pragma endregion

CScene::CScene(
	unsigned int sceneID,
	std::string source
) {
	_id = sceneID;
	_source = source;
}

void CScene::Add(
	unsigned int gameObjectID
) {
	if (std::find(_gameObjects.begin(), _gameObjects.end(), gameObjectID) != _gameObjects.end()) {
		DebugOut(L"[Engine] GameObject with id %d already added to scene.\n", gameObjectID);
		return;
	}

	_gameObjects.push_back(gameObjectID);
}

void CScene::Remove(
	unsigned int gameObjectID
) {
	if (std::find(_gameObjects.begin(), _gameObjects.end(), gameObjectID) == _gameObjects.end()) {
		DebugOut(L"[Engine] GameObject with id %d not existed in scene.\n", gameObjectID);
		return;
	}

	_gameObjects.erase(
		std::remove(_gameObjects.begin(), _gameObjects.end(), gameObjectID),
		_gameObjects.end()
	);
}

void CScene::Clean() {
	_gameObjects.clear();
}

CGame::CGame() {
	_application = new CApplication();
	_time = new CTime();
	_graphics = new CGraphics();
	_audio = new CAudio();
	_input = new CInput();
}

CGame::~CGame() {
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

void CGame::KeyState() {
	_previousButtonState = _currentButtonState;
}

void CGame::OnKeyUp(
	int keyCode
) {
	if (_currentButtonState.find(keyCode) != _currentButtonState.end()) {
		_currentButtonState[keyCode] = false;
	}
}

void CGame::OnKeyDown(
	int keyCode
) {
	if (_currentButtonState.find(keyCode) != _currentButtonState.end()) {
		_currentButtonState[keyCode] = true;
	}
}

void CGame::BindKey(
	int keyCode
) {
	_previousButtonState[keyCode] = false;
	_currentButtonState[keyCode] = false;
}

bool CGame::IsKeyDown(
	int keyCode
) {
	return _currentButtonState[keyCode];
}

bool CGame::IsKeyPressed(
	int keyCode
) {
	return
		!_previousButtonState[keyCode]
		&& _currentButtonState[keyCode];
}

bool CGame::IsKeyReleased(
	int keyCode
) {
	return
		_previousButtonState[keyCode]
		&& !_currentButtonState[keyCode];
}

void CGame::AddScene(
	unsigned int sceneID, 
	std::string source
) {
	if (_scenes.find(sceneID) != _scenes.end()) {
		DebugOut(L"[Engine] Scene id already used: %d - %s.\n", sceneID, source);
		return;
	}

	_scenes[sceneID] = new CScene(sceneID, source);
}

void CGame::SceneLoading() {
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

void CGame::LoadScene(
	pScene scene
) {
	scene->_load = false;

	pugi::xml_document sceneDoc;
	sceneDoc.load_file(scene->_source.c_str());

	for (auto gameObjectNode = sceneDoc.child("Scene").child("GameObject");
		gameObjectNode;
		gameObjectNode = gameObjectNode.next_sibling("GameObject")) {
		Create(
			scene,
			gameObjectNode.attribute("actor").as_uint(),
			gameObjectNode.attribute("name").as_string(),
			gameObjectNode.attribute("source").as_string(),
			gameObjectNode.attribute("x").as_float(),
			gameObjectNode.attribute("y").as_float(),
			gameObjectNode.attribute("gx").as_int(),
			gameObjectNode.attribute("gy").as_int(),
			gameObjectNode.attribute("layer").as_int()
		);
	}
}

void CGame::UnloadScene(
	pScene scene
) {
	scene->_load = false;

	for (auto gameObjectID : scene->_gameObjects) {
		_gameObjects[gameObjectID]->Destroy();
	}
}

void CGame::PlayScene(
	unsigned int sceneID
) {
	if (_scenes.find(sceneID) == _scenes.end()) {
		DebugOut(L"[Engine] Scene id non existed: %d.\n", sceneID);
		return;
	}

	_sceneLoading = true;
	_scenes[sceneID]->_load = true;
	_scenes[sceneID]->_play = true;
}

void CGame::StopScene(
	unsigned int sceneID
) {
	if (_scenes.find(sceneID) == _scenes.end()) {
		DebugOut(L"[Engine] Scene id non exist: %d.\n", sceneID);
		return;
	}

	_sceneLoading = true;
	_scenes[sceneID]->_load = true;
	_scenes[sceneID]->_play = false;
}

void CGame::Purge() {
	for (auto gameObject = _gameObjects.begin(); gameObject != _gameObjects.end();) {
		if (gameObject->second->IsDestroyed()) {
			RemoveGrid(gameObject->first);

			gameObject->second->GetScene()->Remove(
				gameObject->first
			);

			_dictionary.erase(gameObject->second->GetName());

			delete gameObject->second;
			gameObject->second = nullptr;

			gameObject = _gameObjects.erase(gameObject);
		}
		else {
			gameObject++;
		}
	}
}

void CGame::AddGameObject(
	pGameObject gameObject
) {
	_gameObjects[gameObject->GetId()] = gameObject;
	_dictionary[gameObject->GetName()] = gameObject->GetId();

	AddGrid(gameObject->GetId());
	gameObject->GetScene()->Add(gameObject->GetId());
}

pGameObject CGame::GetGameObject(
	unsigned int gameObjectID
) {
	if (_gameObjects.find(gameObjectID) != _gameObjects.end()) {
		return _gameObjects[gameObjectID];
	}

	return nullptr;
}

pGameObject CGame::GetGameObject(
	std::string gameObjectName
) {
	if (_dictionary.find(gameObjectName) != _dictionary.end()) {
		return GetGameObject(_dictionary[gameObjectName]);
	}

	return nullptr;
}

std::vector<unsigned int> CGame::GetActives() {
	return _updateQueue;
}

void CGame::AddGrid(
	unsigned int gameObjectID
) {
	int gridX = 0;
	int gridY = 0;

	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	if (_grid.find(cell) == _grid.end()) {
		_grid[cell] = { gameObjectID };
	}
	else {
		_grid[cell].push_back(gameObjectID);
	}
}

void CGame::RemoveGrid(
	unsigned int gameObjectID
) {
	int gridX = 0;
	int gridY = 0;

	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);
	auto cell = std::make_pair(gridX, gridY);

	std::vector<unsigned int>& it = _grid.find(cell)->second;
	it.erase(
		std::remove(it.begin(), it.end(), gameObjectID),
		it.end()
	);
}

void CGame::UpdateGrid(
	unsigned int gameObjectID
) {
	auto gameObject = _gameObjects[gameObjectID];

	float x = 0;
	float y = 0;
	gameObject->GetPosition(x, y);

	int gx = 0;
	int gy = 0;
	gameObject->GetGrid(gx, gy);

	int newGx = 0;
	int newGy = 0;

	if (int(x) > (_gridWidth / 2)) {
		newGx = (int(x - (_gridWidth / 2)) / _gridWidth) + 1;
	}
	else if (int(x) < (-_gridWidth / 2)) {
		newGx = (int(x + (_gridWidth / 2)) / _gridWidth) - 1;
	}

	if (int(y) > (_gridHeight / 2)) {
		newGy = (int(y - (_gridHeight / 2)) / _gridHeight) + 1;
	}
	else if (int(y) < (-_gridHeight / 2)) {
		newGy = (int(y + (_gridHeight / 2)) / _gridHeight) - 1;
	}

	if (newGx != gx || newGy != gy) {
		RemoveGrid(gameObjectID);
		gameObject->SetGrid(newGx, newGy);
		AddGrid(gameObjectID);
	}
}

std::vector<pGameObject> CGame::GetLocal(
	unsigned int gameObjectID
) {
	std::vector<unsigned int> local;
	std::vector<pGameObject> gameObjects;

	int gridX = 0;
	int gridY = 0;
	_gameObjects[gameObjectID]->GetGrid(gridX, gridY);

	/* left-top */
	auto cell = std::make_pair(gridX - 1, gridY + 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* top */
	cell = std::make_pair(gridX, gridY + 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* right-top */
	cell = std::make_pair(gridX + 1, gridY + 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* left */
	cell = std::make_pair(gridX - 1, gridY);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* center */
	cell = std::make_pair(gridX, gridY);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* right */
	cell = std::make_pair(gridX + 1, gridY);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* left-bottom */
	cell = std::make_pair(gridX - 1, gridY - 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* bottom */
	cell = std::make_pair(gridX, gridY - 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	/* right-bottom */
	cell = std::make_pair(gridX + 1, gridY - 1);
	for (auto gameObject : _grid[cell]) {
		local.push_back(gameObject);
	}

	for (auto gameObject : local) {
		if (gameObject != gameObjectID
			&& !_gameObjects[gameObject]->IsDestroyed()) {
			gameObjects.push_back(_gameObjects[gameObject]);
		}
	}

	return gameObjects;
}

void CGame::Run(
	HINSTANCE hInstance,
	std::string source
) {
	if (!Load(hInstance, source)) {
		Shutdown();
		return;
	}

	_time->StartClock();
	float msPerFrame = 1000.0f / _frameRate;
	float elapsedMs = 0.0f;

	while (!_application->HandleMessage()) {
		SceneLoading();

		_time->TickClock();
		elapsedMs += _time->GetElapsedMs();

		if (elapsedMs >= msPerFrame) {
			_input->ProcessKeyboard();
			Update(elapsedMs);
			Render();
			Purge();
			elapsedMs = 0.0f;
		}
		else {
			Sleep(DWORD(msPerFrame - elapsedMs));
		}
	}

	Shutdown();
}

bool CGame::Load(
	HINSTANCE hInstance,
	std::string source
) {
	bool result;

	pugi::xml_document gameData;
	gameData.load_file(source.c_str());

	auto gameSettingsNode = gameData.child("GameData").child("GameSettings");

	_frameRate = gameSettingsNode.attribute("frameRate").as_float();
	_gridWidth = gameSettingsNode.attribute("gridWidth").as_int();
	_gridHeight = gameSettingsNode.attribute("gridHeight").as_int();
	_cameraBuffer = gameSettingsNode.attribute("cameraBuffer").as_float();

	result = _application->CreateGameWindow(
		hInstance,
		std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
			gameSettingsNode.attribute("title").as_string()
		),
		gameSettingsNode.attribute("resolutionWidth").as_uint(),
		gameSettingsNode.attribute("resolutionHeight").as_uint()
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

	for (auto buttonNode = gameData.child("GameData").child("Button");
		buttonNode;
		buttonNode = buttonNode.next_sibling("Button")) {
		BindKey(
			buttonNode.attribute("keyCode").as_int()
		);
	}

	for (auto textureNode = gameData.child("GameData").child("Texture");
		textureNode;
		textureNode = textureNode.next_sibling("Texture")) {
		_graphics->LoadTexture(
			textureNode.attribute("id").as_uint(),
			std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(
				textureNode.attribute("source").as_string()
			)
		);
	}

	for (auto sceneNode = gameData.child("GameData").child("Scene");
		sceneNode;
		sceneNode = sceneNode.next_sibling("Scene")) {
		AddScene(
			sceneNode.attribute("id").as_uint(),
			sceneNode.attribute("source").as_string()
		);
	}

	PlayScene(
		gameSettingsNode.attribute("startScene").as_uint()
	);

	return true;
}

void CGame::Update(float elapsedMs) {
	_updateQueue.clear();
	_renderQueue.clear();

	for (auto gameObject : _gameObjects) {
		if (!gameObject.second->IsDestroyed()) {
			_updateQueue.push_back(gameObject.first);
			_renderQueue.push_back(gameObject.second);
		}
	}

	for (auto gameObjectID : _updateQueue) {
		_gameObjects[gameObjectID]->Update(elapsedMs);
		UpdateGrid(gameObjectID);
	}

	std::sort(_renderQueue.begin(), _renderQueue.end(), CGameObject::CompareLayer);
}

void CGame::Render() {
	_graphics->GetDevice()->ClearRenderTargetView(
		_graphics->GetRenderTargetView(),
		D3DXCOLOR(200.0f / 255, 200.0f / 255, 255.0f / 255, 0.0f)
	);
	_graphics->GetSpriteHandler()->Begin(D3DX10_SPRITE_SORT_TEXTURE);

	FLOAT NewBlendFactor[4] = { 0,0,0,0 };
	_graphics->GetDevice()->OMSetBlendState(_graphics->GetBlendStateAlpha(), NewBlendFactor, 0xffffffff);

	for (auto& gameObject : _renderQueue) {
		gameObject->Render();
	}

	_graphics->GetSpriteHandler()->End();
	_graphics->GetSwapChain()->Present(0, 0);
}

void CGame::Shutdown() {
	_updateQueue.clear();
	_renderQueue.clear();

	for (auto& gameObject : _gameObjects) {
		delete gameObject.second;
		gameObject.second = nullptr;
	}
	_gameObjects.clear();
	_dictionary.clear();
	
	for (auto& cell : _grid) {
		cell.second.clear();
	}
	_grid.clear();

	for (auto& scene : _scenes) {
		scene.second->Clean();
		delete scene.second;
		scene.second = nullptr;
	}
	_scenes.clear();

	_input->Shutdown();
	_audio->Shutdown();
	_graphics->Shutdown();
}