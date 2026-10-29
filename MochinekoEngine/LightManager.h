#pragma once
#include <string>
#include <DirectXMath.h>

namespace LightManager {

	void Init();
	DirectX::XMFLOAT3 GetDefaultLight();

	DirectX::XMFLOAT3 GetLight(const std::string& lightName);
	void SetLight(const std::string& lightName, const DirectX::XMFLOAT3& lightDirection);

	bool IsLightModelDraw();
	void SetLightModelDraw(bool flag);

	void DrawImGUI();

}