#pragma once
#ifndef __AUDIO_H__
#define __AUDIO_H__

#pragma region INCLUDE
#include <dsound.h>
#include <unordered_map>
#pragma endregion

struct Sound {
	LPDIRECTSOUNDBUFFER8 _secondaryBuffer = NULL;

	~Sound();
	bool Play();
};
typedef Sound* pSound;

class Audio {

#pragma region DirectSound Wrapper
private:
	struct WaveHeaderType {
		char chunkId[4];
		unsigned long chunkSize;
		char format[4];
		char subChunkId[4];
		unsigned long subChunkSize;
		unsigned short audioFormat;
		unsigned short numChannels;
		unsigned long sampleRate;
		unsigned long bytesPerSecond;
		unsigned short blockAlign;
		unsigned short bitsPerSample;
		char dataChunkId[4];
		unsigned long dataSize;
	};

private:
	LPDIRECTSOUND8 _device = NULL;
	LPDIRECTSOUNDBUFFER _primaryBuffer = NULL;

public:
	LPDIRECTSOUND8 GetDevice() { return _device; }

private:
	pSound LoadSound(std::string sourcePath);

public:
	bool Initialize(HWND hWnd);
	void Shutdown();
#pragma endregion

#pragma region Sounds Database
private:
	std::unordered_map<unsigned int, pSound> _sounds;

public:
	void Load(unsigned int id, std::string sourcePath);
	pSound GetSound(unsigned int id);
#pragma endregion

};
typedef Audio* pAudio;

#endif // !__AUDIO_H__
