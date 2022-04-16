#pragma region INCLUDE
#include "Animation.h"
#include "Sprite.h"
#include "GameObject.h"
#pragma endregion

CAnimation::CAnimation(
	pGameObject gameObject
) {
	_gameObject = gameObject;
}

CAnimation::~CAnimation() {
	_frames.clear();
	_gameObject = nullptr;
}

void CAnimation::AddFrame(
	unsigned int spriteID,
	float frameTime
) {
	_frames.push_back(
		std::make_pair(spriteID, frameTime)
	);
}

void CAnimation::Play(
	bool loop
) {
	_play = true;
	_loop = loop;
	_frameIndex = 0;
	_elapsedTime = 0.0f;
}

void CAnimation::Stop() {
	_play = false;
	_loop = false;
	_frameIndex = 0;
	_elapsedTime = 0.0f;
}

void CAnimation::Update(
	float elapsedMs
) {
	if (!_play) {
		return;
	}

	_elapsedTime += elapsedMs;

	while (_elapsedTime >= _frames.at(_frameIndex).second) {
		_elapsedTime -= _frames.at(_frameIndex).second;
		_frameIndex++;

		if (_frameIndex < _frames.size()) {
			continue;
		}

		if (_loop) {
			_frameIndex = 0;
		}
		else {
			Stop();
		}
	}
}

void CAnimation::Render(float x, float y, bool isUI) {
	_gameObject->GetSprite(
		_frames.at(_frameIndex).first
	)->Render(x, y, isUI);
}
