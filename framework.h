#pragma once
#include <Windows.h>

namespace GameWindow {
	inline const int WINDOW_WIDTH = 1280;
	inline const int WINDOW_HEIGHT = 720;
	inline const char* WINDOW_CLASS = "GameEngine";
	inline const char* WINDOW_TITLE = "Game";

	inline HWND mainHWND = {};
}