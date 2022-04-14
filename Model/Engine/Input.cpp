#pragma region INCLUDE
#include "Input.h"
#include "Ultilities.h"
#pragma endregion

bool CInput::Initialize(
	HINSTANCE hInstance,
	HWND hWnd,
	pKeyHandler keyHandler
) {
	_keyHandler = keyHandler;

	HRESULT result = DirectInput8Create(
		hInstance,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(VOID**)&_directInput, NULL
	);
	if (result != DI_OK) {
		return false;
	}

	result = _directInput->CreateDevice(
		GUID_SysKeyboard,
		&_device,
		NULL
	);
	if (result != DI_OK) {
		return false;
	}

	result = _device->SetDataFormat(
		&c_dfDIKeyboard
	);
	if (result != DI_OK) {
		return false;
	}

	result = _device->SetCooperativeLevel(
		hWnd,
		DISCL_FOREGROUND | DISCL_NONEXCLUSIVE
	);
	if (result != DI_OK) {
		return false;
	}

	DIPROPDWORD dipdw;
	dipdw.diph.dwSize = sizeof(DIPROPDWORD);
	dipdw.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	dipdw.diph.dwObj = 0;
	dipdw.diph.dwHow = DIPH_DEVICE;
	dipdw.dwData = KEYBOARD_BUFFER_SIZE;

	result = _device->SetProperty(
		DIPROP_BUFFERSIZE,
		&dipdw.diph
	);
	if (result != DI_OK) {
		return false;
	}

	result = _device->Acquire();
	if (result != DI_OK) {
		return false;
	}

	return true;
}

void CInput::Shutdown() {
	if (_device) {
		_device->Unacquire();
		_device->Release();
	}

	if (_directInput) {
		_directInput->Release();
	}
}

void CInput::ProcessKeyboard() {
	HRESULT result = _device->GetDeviceState(
		sizeof(_keyStates),
		_keyStates
	);
	if (FAILED(result)) {
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED)) {
			HRESULT h = _device->Acquire();
			if (h != DI_OK) return;
		}
		else return;
	}

	_keyHandler->KeyState();

	DWORD dwElements = KEYBOARD_BUFFER_SIZE;
	result = _device->GetDeviceData(
		sizeof(DIDEVICEOBJECTDATA),
		_keyEvents,
		&dwElements,
		0
	);
	if (FAILED(result)) {
		return;
	}

	for (DWORD i = 0; i < dwElements; i++) {
		int KeyCode = _keyEvents[i].dwOfs;
		int KeyState = _keyEvents[i].dwData;
		if ((KeyState & 0x80) > 0)
			_keyHandler->OnKeyDown(KeyCode);
		else
			_keyHandler->OnKeyUp(KeyCode);
	}
}
