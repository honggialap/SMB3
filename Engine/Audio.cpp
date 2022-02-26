#pragma region INCLUDE
#include "Audio.h"
#include "Ultilities.h"
#pragma endregion

namespace Engine
{

	bool Audio::Initialize(
		HWND hWnd
	)
	{
		HRESULT result;

		result = DirectSoundCreate8(
			NULL, 
			&_device, 
			NULL
		);
		if (FAILED(result))
		{
			return false;
		}

		result = _device->SetCooperativeLevel(
			hWnd, 
			DSSCL_PRIORITY
		);
		if (FAILED(result))
		{
			return false;
		}

		DSBUFFERDESC bufferDesc;
		bufferDesc.dwSize = sizeof(DSBUFFERDESC);
		bufferDesc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
		bufferDesc.dwBufferBytes = 0;
		bufferDesc.dwReserved = 0;
		bufferDesc.lpwfxFormat = NULL;
		bufferDesc.guid3DAlgorithm = GUID_NULL;

		result = _device->CreateSoundBuffer(
			&bufferDesc, 
			&_primaryBuffer, 
			NULL
		);
		if (FAILED(result))
		{
			return false;
		}

		WAVEFORMATEX waveFormat;
		waveFormat.wFormatTag = WAVE_FORMAT_PCM;
		waveFormat.nSamplesPerSec = 44100;
		waveFormat.wBitsPerSample = 16;
		waveFormat.nChannels = 2;
		waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
		waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
		waveFormat.cbSize = 0;

		result = _primaryBuffer->SetFormat(
			&waveFormat
		);
		if (FAILED(result))
		{
			return false;
		}

		return true;
	}

	void Audio::Shutdown()
	{
		if (_primaryBuffer)
		{
			_primaryBuffer->Release();
		}
		
		if (_device)
		{
			_device->Release();
		}
	}

}
