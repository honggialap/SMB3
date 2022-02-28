#pragma region INCLUDE
#include "Scene.h"
#include "Ultilities.h"
#include "Game.h"
#include "GameObject.h"
#include <algorithm>
#pragma endregion

namespace Engine
{

	void Scene::AddGameObject(unsigned int id)
	{
		if (
			std::find(_gameObjects.begin(), _gameObjects.end(), id)
			!= _gameObjects.end()
			)
		{
			DebugOut(L"[Engine] GameObject with id %d already added to scene.\n", id);
			return;
		}

		_gameObjects.push_back(id);
	}

	void Scene::RemoveGameObject(unsigned int id)
	{
		if (
			std::find(_gameObjects.begin(), _gameObjects.end(), id)
			== _gameObjects.end()
			)
		{
			DebugOut(L"[Engine] GameObject with id %d not existed in scene.\n", id);
			return;
		}

		_gameObjects.erase(
			std::remove(_gameObjects.begin(), _gameObjects.end(), id), 
			_gameObjects.end()
		);
	}

	void Scenes::AddScene(unsigned int id, std::string source)
	{
		if (_scenes.find(id) != _scenes.end())
		{
			DebugOut(L"[Engine] Scene id already used: %d - %s.\n", id, source);
			return;
		}

		_scenes[id] = new Scene(source);
	}

	void Scenes::Loading()
	{
		if (!_loading)
		{
			return;
		}

		for (auto& scene : _scenes)
		{
			if (!scene.second->_load)
			{
				continue;
			}

			if (scene.second->_play)
			{
				LoadScene(scene.second);
			}
			else
			{
				UnloadScene(scene.second);
			}
		}

		_loading = false;
	}

	void Scenes::LoadScene(pScene scene)
	{
		scene->_load = false;

		// call game to create game object
		// add created game object to scene
	}

	void Scenes::UnloadScene(pScene scene)
	{
		scene->_load = false;

		// call game object destroy function
		// remove destroyed game object from scene
		// all destroyed game objects will be purge
	}

	void Scenes::PlayScene(unsigned int id)
	{
		if (_scenes.find(id) == _scenes.end())
		{
			DebugOut(L"[Engine] Scene id non existed: %d.\n", id);
			return;
		}

		_loading = true;
		_scenes[id]->_load = true;
		_scenes[id]->_play = true;
	}

	void Scenes::StopScene(unsigned int id)
	{
		if (_scenes.find(id) == _scenes.end())
		{
			DebugOut(L"[Engine] Scene id non exist: %d.\n", id);
			return;
		}

		_loading = true;
		_scenes[id]->_load = true;
		_scenes[id]->_play = false;
	}

	void Scenes::Shutdown()
	{
		for (auto& scene : _scenes)
		{
			delete scene.second;
			scene.second = nullptr;
		}
		_scenes.clear();
	}

}
