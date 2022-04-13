#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#pragma region FORWARD DECLARATION
class CGameObject;
typedef CGameObject* pGameObject;
#pragma endregion

#pragma region INCLUDE
#include <vector>
#pragma endregion

class CAnimation {
private:
	pGameObject _gameObject;
	std::vector<std::pair<unsigned int, float>> _frames;
	unsigned int _frameIndex = 0;
	float _elapsedTime = 0.0f;
	bool _play = false;
	bool _loop = false;

public:
	CAnimation(pGameObject gameObject);
	~CAnimation();

public:
	void AddFrame(unsigned int spriteId, float frameTime);

	void Play(bool loop = false);
	void Stop();

	void Update(float elapsedMs);
	void Render(float x, float y, bool isUI = false);
};
typedef CAnimation* pAnimation;

#endif // !__ANIMATION_H__
