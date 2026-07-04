#include <Windows.h>

namespace GameLib {
	inline HWND mainWindowHandle_ = {};

	HWND GetWindowHandle() {
		return mainWindowHandle_;
	}
}

using namespace GameLib;

void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow);
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	InitWindow(hInstance, hPrevInstance, lpCmdLine, nCmdShow);

	MSG msg = {};
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {

		}
	}

	return 0;
}

void InitWindow(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {

	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof(WNDCLASSEX); 
	wndClass.hInstance = hInstance; 
	wndClass.lpszClassName = "Game";
	wndClass.lpfnWndProc = WndProc;
	wndClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	wndClass.hIconSm = LoadIcon(nullptr, IDI_WINLOGO);
	wndClass.hCursor = LoadCursor(nullptr, IDC_ARROW); 
	wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	RegisterClassEx(&wndClass);

	mainWindowHandle_ = CreateWindow(
		"Game",
		"GameTitle", 
		WS_OVERLAPPEDWINDOW, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		1280, 720,
		NULL, 
		NULL, 
		hInstance, 
		NULL 
	);

	ShowWindow(mainWindowHandle_, nCmdShow);
	UpdateWindow(mainWindowHandle_);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
		case WM_DESTROY: {
			PostQuitMessage(0);
			break;
		}
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}