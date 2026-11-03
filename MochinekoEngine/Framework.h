#pragma once
#include <Windows.h>
#include <array>
#include <functional>
#include <string>

struct Color;

namespace MochinekoEngine {

#ifdef _DEBUG
	const bool DEBUG_MODE = true;
#else
	const bool DEBUG_MODE = false;
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

namespace MochinekoUtil {

	/// <summary>
	/// Win32の古いファイルダイアログが開きます
	/// </summary>
	/// <param name="filter"></param>
	/// <param name="runAction">成功したときの実行したい処理</param>
	void openFileDialog(const char* filter, std::function<void(const std::string& path)> runAction);
}

namespace MochinekoEngineResource {
	inline int sphereColHandle_ = -1;
	inline int boxColHandle_ = -1;

	void Init();
}