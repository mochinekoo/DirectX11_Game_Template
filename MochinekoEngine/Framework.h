#pragma once
#include <Windows.h>
#include <array>

struct Color;

namespace MochinekoEngine {

#ifdef _DEBUG
	const bool DEBUG_MODE = true;
#else
	const bool DEBUG_MODE = false
#endif

	HWND GetGameWindowHandle();
	bool CanShutdown();
	void Shutdown();
	float GetDeltaTime();
	Color GetBackgroundColor();
	void SetBackgroundColor(const Color& color);


#ifdef _DEBUG
	bool IsShowFileChoose();
	void SetFileChoose(bool flag);
#endif
}

namespace MochinekoEngineResource {
	inline int sphereColHandle_ = -1;
	inline int boxColHandle_ = -1;

	void Init();
}