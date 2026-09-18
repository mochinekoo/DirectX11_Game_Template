#pragma once
#include <Windows.h>
#include <array>

struct Color;

namespace MochinekoEngine {

	HWND GetGameWindowHandle();
	bool CanShutdown();
	void Shutdown();
	float GetDeltaTime();
	Color GetBackgroundColor();
	void SetBackgroundColor(const Color& color);
}

namespace MochinekoEngineResource {
	inline int sphereColHandle_ = -1;
	inline int boxColHandle_ = -1;

	void Init();
}