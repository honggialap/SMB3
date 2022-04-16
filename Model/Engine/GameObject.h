#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#pragma region FORWARD DECLARATION
class CGame;
typedef CGame* pGame;

struct CScene;
typedef CScene* pScene;
#pragma endregion

#pragma region INCLUDE
#include "Sprite.h"
#include "Animation.h"
#include "Collision.h"
#include "Sound.h"
#pragma endregion

class CGameObject {
protected:
	pGame _game = nullptr;
	pScene _scene = nullptr;
	unsigned int _ID = 0;
	std::string _name;
	std::string _source;

	bool _start = false;
	bool _destroy = false;

	float _x = 0;
	float _y = 0;

	float _vx = 0;
	float _vy = 0;

	int _gx = 0;
	int _gy = 0;

	unsigned int _layer = 0;

	std::unordered_map<unsigned int, pSprite> _sprites;
	std::unordered_map<unsigned int, pAnimation> _animations;
	std::unordered_map<unsigned int, pSound> _sounds;
	pCollider _collider = nullptr;

public:
	CGameObject(
		pGame game, pScene scene,
		unsigned int ID, std::string name, std::string source,
		float x, float y,
		int gx, int gy,
		unsigned int layer
	);
	~CGameObject();

	pGame GetGame() { return _game; }
	pScene GetScene() { return _scene; }
	unsigned int GetID() { return _ID; }
	std::string GetName() { return _name; }

	bool IsDestroyed() { return _destroy; }

	virtual void Load();
	virtual void Start() { _start = true; }
	virtual void Update(float elapsedMs) = 0;
	virtual void Render() = 0;
	void Destroy() { _destroy = true; }

	void SetPosition(float x, float y) { _x = x; _y = y; }
	void GetPosition(float& x, float& y) { x = _x; y = _y; }

	void SetVelocity(float vx, float vy) { _vx = vx; _vy = vy; }
	void GetVelocity(float& vx, float& vy) { vx = _vx; vy = _vy; }

	void GetGrid(int& gx, int& gy) { gx = _gx; gy = _gy; }
	void SetGrid(int gx, int gy) { _gx = gx; _gy = gy; }

	unsigned int GetLayer() { return _layer; }
	void SetLayer(unsigned int layer) { _layer = layer; }
	static bool CompareLayer(CGameObject* a, CGameObject* b) { return a->_layer < b->_layer; }

	void AddSprite(unsigned int id, pSprite sprite);
	pSprite GetSprite(unsigned int id) { return _sprites[id]; }

	void AddAnimation(unsigned int id, pAnimation animation);
	pAnimation GetAnimation(unsigned int id) { return _animations[id]; }
	
	void AddSound(unsigned int id, pSound sound);
	pSound GetSound(unsigned int id) { return _sounds[id]; }

	pCollider GetCollider() { return _collider; }

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(float elapsedMs) {};
	virtual void OnCollisionWith(pCollision e) {};
};
typedef CGameObject* pGameObject;

#endif // !__GAME_OBJECT_H__
