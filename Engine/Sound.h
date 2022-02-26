#pragma once
#ifndef __SOUND_H__
#define __SOUND_H__

#pragma region INCLUDE
#include "Audio.h"
#include <unordered_map>
#pragma endregion

namespace Engine
{

	struct Sound
	{
		LPDIRECTSOUNDBUFFER8 _secondaryBuffer = NULL;

		~Sound();
		bool Play();
	};
	typedef Sound* pSound;

	class Sounds
	{
	private:
		struct WaveHeaderType
		{
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

		pAudio _audio;
		std::unordered_map<unsigned int, pSound> _sounds;
		
	public:
		Sounds(
			pAudio audio
		);
		~Sounds();

		void Load(
			unsigned int id,
			std::string sourcePath
		);
		pSound LoadSound(
			std::string sourcePath
		);
		void AddSound(
			unsigned int id,
			pSound sound
		);
		pSound GetSound(
			unsigned int id
		);
		void Clean();
	};
	typedef Sounds* pSounds;

}

#endif // !__SOUND_H__
