#pragma once
#ifndef __SOUND_H__
#define	__SOUND_H__

#pragma region INCLUDE
#include "Audio.h"
#pragma endregion

#pragma region FORWARD DECLARATION
class CGameObject;
typedef CGameObject* pGameObject;
#pragma endregion

class CSound {
private:
	pGameObject _gameObject;
	LPDIRECTSOUNDBUFFER8 _secondaryBuffer = NULL;

public:
	LPDIRECTSOUNDBUFFER8 GetBuffer() { return _secondaryBuffer; }

public:
	CSound(pGameObject gameObject);
	~CSound();

	void Play(bool global, bool loop);
};
typedef CSound* pSound;

#endif // !__SOUND_H__