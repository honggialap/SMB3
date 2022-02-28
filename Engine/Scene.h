#pragma once
#ifndef __SCENE_H__
#define __SCENE_H__

#pragma region INCLUDE
#include <vector>
#include <map>
#include <string>
#pragma endregion

namespace Engine
{

#pragma region FORWARD DECLERATION
	class Game;
	typedef Game* pGame;
#pragma endregion

	struct Scene
	{
		std::string _source;
		std::vector<unsigned int> _gameObjects;
		bool _play = false;
		bool _load = false;

		Scene(std::string source)
		{
			_source = source;
		}

		void AddGameObject(unsigned int id);
		void RemoveGameObject(unsigned int id);
	};
	typedef Scene* pScene;

	class Scenes
	{
	private:
		pGame _game;
		std::map<unsigned int, pScene> _scenes;
		bool _loading = false;

	public:
		Scenes(pGame game) { _game = game; }
		void AddScene(unsigned int id, std::string source);

		void Loading();
		void LoadScene(pScene scene);
		void UnloadScene(pScene scene);

		void PlayScene(unsigned int id);
		void StopScene(unsigned int id);

		void Shutdown();
	};
	typedef Scenes* pScenes;

}

#endif // !__SCENE_H__
