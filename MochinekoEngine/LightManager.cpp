#include "LightManager.h"
#include <string>
#include <map>
#include <DirectXMath.h>
#include "ImGUI/imgui.h"

using namespace DirectX;

namespace {
	bool drawLightModel_ = false;
	XMFLOAT3 DEFAULT_LIGHT = { 0.0f, -1.0f, 0.0f};
	std::map<std::string, XMFLOAT3> lightMap_;
}

void LightManager::Init() {
	lightMap_.clear();
}

DirectX::XMFLOAT3 LightManager::GetDefaultLight() {
	return DEFAULT_LIGHT;
}

DirectX::XMFLOAT3 LightManager::GetLight(const std::string& lightName) {
	return lightMap_[lightName];
}

void LightManager::SetLight(const std::string& lightName, const DirectX::XMFLOAT3& lightDirection) {
	lightMap_[lightName] = lightDirection;
}

bool LightManager::IsLightModelDraw() {
	return drawLightModel_;
}

void LightManager::SetLightModelDraw(bool flag) {

}

void LightManager::DrawImGUI() {
	ImGui::Begin("LightManager");
	ImGui::Text("Default Light");
	ImGui::SliderFloat("Direction X", &DEFAULT_LIGHT.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Direction Y", &DEFAULT_LIGHT.y, -1.0f, 1.0f);
	ImGui::SliderFloat("Direction Z", &DEFAULT_LIGHT.z, -1.0f, 1.0f);

	for (auto& light : lightMap_) {
		auto& name = light.first;
		auto& lightDirection = light.second;
		ImGui::Text(name.c_str());
		ImGui::SliderFloat("Direction X", &lightDirection.x, -1.0f, 1.0f);
		ImGui::SliderFloat("Direction Y", &lightDirection.y, -1.0f, 1.0f);
		ImGui::SliderFloat("Direction Z", &lightDirection.z, -1.0f, 1.0f);
	}
	ImGui::End();
}
