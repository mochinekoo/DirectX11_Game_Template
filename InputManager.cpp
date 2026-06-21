#include "InputManager.h"

namespace {
	inline LPDIRECTINPUT8 g_pDI;
	inline LPDIRECTINPUTDEVICE8 g_pKeyboard;
	inline DIDATAFORMAT format;
	inline BYTE keys[256] = {};
}

void InputManager::Initialize(HINSTANCE hinstance, HWND hwnd) {
	DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&g_pDI, NULL);
	g_pDI->CreateDevice(GUID_SysKeyboard, &g_pKeyboard, NULL);

	g_pKeyboard->SetDataFormat(&c_dfDIKeyboard);
	g_pKeyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
}

void InputManager::SetInputState() {
	HRESULT hr = g_pKeyboard->GetDeviceState(sizeof(keys), keys);

	if (FAILED(hr)) {
		g_pKeyboard->Acquire();
	}
}

bool InputManager::GetInputState(int key)
{
	if (keys[key] & 0x80) return true;
	return false;
}
