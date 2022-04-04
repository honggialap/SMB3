#pragma region INCLUDE
#include "Audio.h"
#include "Ultilities.h"
#pragma endregion

#pragma region Sound

Sound::~Sound() {
	if (_secondaryBuffer) {
		_secondaryBuffer->Release();
	}
}

bool Sound::Play() {
	HRESULT result;

	result = _secondaryBuffer->SetCurrentPosition(0);
	if (FAILED(result)) {
		return false;
	}

	result = _secondaryBuffer->SetVolume(DSBVOLUME_MAX);
	if (FAILED(result)) {
		return false;
	}

	result = _secondaryBuffer->Play(0, 0, 0);
	if (FAILED(result)) {
		return false;
	}

	return true;
}

#pragma endregion

#pragma region DirectSound Wrapper

pSound Audio::LoadSound(
	std::string sourcePath
) {
	FILE* filePtr;
	int error;
	error = fopen_s(&filePtr, sourcePath.c_str(), "rb");
	if (error != 0) {
		return nullptr;
	}

	unsigned int count;
	WaveHeaderType waveFileHeader;
	count = fread(&waveFileHeader, sizeof(waveFileHeader), 1, filePtr);
	if (count != 1) {
		return nullptr;
	}

	if ((waveFileHeader.chunkId[0] != 'R') || (waveFileHeader.chunkId[1] != 'I') ||
		(waveFileHeader.chunkId[2] != 'F') || (waveFileHeader.chunkId[3] != 'F')) {
		return nullptr;
	}

	if ((waveFileHeader.format[0] != 'W') || (waveFileHeader.format[1] != 'A') ||
		(waveFileHeader.format[2] != 'V') || (waveFileHeader.format[3] != 'E')) {
		return nullptr;
	}

	if ((waveFileHeader.subChunkId[0] != 'f') || (waveFileHeader.subChunkId[1] != 'm') ||
		(waveFileHeader.subChunkId[2] != 't') || (waveFileHeader.subChunkId[3] != ' ')) {
		return nullptr;
	}

	if (waveFileHeader.audioFormat != WAVE_FORMAT_PCM) {
		return nullptr;
	}

	if (waveFileHeader.numChannels != 2) {
		return nullptr;
	}

	if (waveFileHeader.sampleRate != 44100) {
		return nullptr;
	}

	if (waveFileHeader.bitsPerSample != 16) {
		return nullptr;
	}

	if ((waveFileHeader.dataChunkId[0] != 'd') || (waveFileHeader.dataChunkId[1] != 'a') ||
		(waveFileHeader.dataChunkId[2] != 't') || (waveFileHeader.dataChunkId[3] != 'a')) {
		return nullptr;
	}

	WAVEFORMATEX waveFormat;
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nChannels = 2;
	waveFormat.nBlockAlign = (waveFormat.wBitsPerSample / 8) * waveFormat.nChannels;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	DSBUFFERDESC bufferDesc;
	bufferDesc.dwSize = sizeof(DSBUFFERDESC);
	bufferDesc.dwFlags = DSBCAPS_CTRLVOLUME;
	bufferDesc.dwBufferBytes = waveFileHeader.dataSize;
	bufferDesc.dwReserved = 0;
	bufferDesc.lpwfxFormat = &waveFormat;
	bufferDesc.guid3DAlgorithm = GUID_NULL;

	HRESULT result;
	IDirectSoundBuffer* tempBuffer;
	result = _device->CreateSoundBuffer(
		&bufferDesc,
		&tempBuffer,
		NULL
	);
	if (FAILED(result)) {
		return nullptr;
	}

	auto sound = new Sound();

	result = tempBuffer->QueryInterface(
		IID_IDirectSoundBuffer8,
		(void**)&*sound->_secondaryBuffer
	);
	if (FAILED(result)) {
		delete sound;
		return nullptr;
	}

	tempBuffer->Release();
	tempBuffer = 0;

	fseek(filePtr, sizeof(WaveHeaderType), SEEK_SET);

	unsigned char* waveData;
	waveData = new unsigned char[waveFileHeader.dataSize];
	if (!waveData) {
		delete sound;
		return nullptr;
	}

	count = fread(waveData, 1, waveFileHeader.dataSize, filePtr);
	if (count != waveFileHeader.dataSize) {
		delete sound;
		return nullptr;
	}

	error = fclose(filePtr);
	if (error != 0) {
		delete sound;
		return nullptr;
	}

	unsigned char* bufferPtr;
	unsigned long bufferSize;
	result = sound->_secondaryBuffer->Lock(
		0,
		waveFileHeader.dataSize,
		(void**)&bufferPtr,
		(DWORD*)&bufferSize,
		NULL,
		0,
		0
	);
	if (FAILED(result)) {
		delete sound;
		return nullptr;
	}

	memcpy(bufferPtr, waveData, waveFileHeader.dataSize);

	result = sound->_secondaryBuffer->Unlock(
		(void*)bufferPtr,
		bufferSize,
		NULL,
		0
	);
	if (FAILED(result)) {
		delete sound;
		return nullptr;
	}

	delete[] waveData;
	waveData = 0;

	return sound;
}

bool Audio::Initialize(
	HWND hWnd
) {
	HRESULT result = DirectSoundCreate8(
		NULL,
		&_device,
		NULL
	);
	if (FAILED(result)) {
		return false;
	}

	result = _device->SetCooperativeLevel(
		hWnd,
		DSSCL_PRIORITY
	);
	if (FAILED(result)) {
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
	if (FAILED(result)) {
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
	if (FAILED(result)) {
		return false;
	}

	return true;
}

void Audio::Shutdown() {
	for (auto& sound : _sounds) {
		delete sound.second;
		sound.second = nullptr;
	}
	_sounds.clear();

	if (_primaryBuffer) {
		_primaryBuffer->Release();
	}

	if (_device) {
		_device->Release();
	}
}

#pragma endregion

#pragma region Sounds Database

void Audio::Load(
	unsigned int id,
	std::string sourcePath
) {
	if (_sounds.find(id) != _sounds.end()) {
		DebugOut(L"[Engine] Sound id is already used: %d.\n", id);
		return;
	}

	auto sound = LoadSound(sourcePath);
	if (sound == nullptr) {
		return;
	}

	_sounds[id] = sound;
}

pSound Audio::GetSound(
	unsigned int id
) {
	if (_sounds.find(id) == _sounds.end()) {
		DebugOut(L"[Engine] Sound not found: %d\n", id);
		return nullptr;
	}

	return _sounds[id];
}

#pragma endregion
