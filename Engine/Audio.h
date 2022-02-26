#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma region INCLUDE
#include <dsound.h>
#pragma endregion

namespace Engine
{
	
	class Audio
	{
	private:
		LPDIRECTSOUND8 _device = NULL;
		LPDIRECTSOUNDBUFFER _primaryBuffer = NULL;

	public:
		LPDIRECTSOUND8 GetDevice() { return _device; }

		bool Initialize(
			HWND hWnd
		);
		void Shutdown();
	};
	typedef Audio* pAudio;

}

#endif // !__AUDIO_H__
