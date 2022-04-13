#pragma region INCLUDE
#include "Application.h"
#include "Ultilities.h"
#pragma endregion

void CApplication::AdjustWindowSize(
	HWND hWnd,
	unsigned int width,
	unsigned int height
) {
	DWORD dwStyle = GetWindowLongPtr(hWnd, GWL_STYLE);
	DWORD dwExStyle = GetWindowLongPtr(hWnd, GWL_EXSTYLE);
	RECT rect = { 0, 0, LONG(width), LONG(height) };
	AdjustWindowRectEx(&rect, dwStyle, FALSE, dwExStyle);
}

void CApplication::RegisterWindowClass(
	HINSTANCE hInstance
) {
	WNDCLASSEX wcex = {};

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.hInstance = hInstance;
	wcex.lpfnWndProc = (WNDPROC)WinProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WINDOW_CLASS_NAME.c_str();
	wcex.hIconSm = NULL;

	RegisterClassEx(&wcex);
}

bool CApplication::CreateGameWindow(
	HINSTANCE hInstance,
	std::wstring title,
	unsigned int width,
	unsigned int height
) {
	RegisterWindowClass(hInstance);

	_hInstance = hInstance;
	_hWnd = CreateWindowEx(
		0,
		WINDOW_CLASS_NAME.c_str(),
		title.c_str(),
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		width, height,
		NULL, NULL,
		hInstance,
		NULL
	);

	if (!_hWnd) {
		DebugOut(L"[Engine] Create game window failed.\n");

		return false;
	}
	else {
		AdjustWindowSize(_hWnd, width, height);
		ShowWindow(_hWnd, SW_SHOWNORMAL);
		UpdateWindow(_hWnd);

		return true;
	}
}

void CApplication::Exit() {
	SendMessage(_hWnd, WM_DESTROY, 0, 0);
}

bool CApplication::HandleMessage() {
	MSG msg = {};
	bool done = false;

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		if (msg.message == WM_QUIT) {
			done = true;
		}

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return done;
}

LRESULT CApplication::WinProc(
	HWND hWnd,
	UINT uMsg,
	WPARAM wParam,
	LPARAM lParam
) {
	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return 0;
}