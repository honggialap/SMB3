#pragma region INCLUDE
#include "Sound.h"
#pragma endregion

CSound::CSound(
	pGameObject gameObject
) {
	_gameObject = gameObject;
}

CSound::~CSound() {
	if (_secondaryBuffer) {
		_secondaryBuffer->Release();
	}
}

void CSound::Play(
	bool global,
	bool loop
) {
	HRESULT result;

	result = _secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result)) {
		return;
	}

	result = _secondaryBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result)) {
		return;
	}

	result = _secondaryBuffer->Play(0, 0, 0);
	if (FAILED(result)) {
		return;
	}
}
