#pragma once
#include <dinput.h>

#pragma comment(lib , "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace InputManager {
	void Initialize(HINSTANCE hinstance, HWND hwnd);
	void SetInputState();
	bool GetInputState(int key);
}