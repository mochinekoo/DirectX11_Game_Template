#include "CameraManager.h"
#include <map>

namespace {
	std::map<std::string, Camera*> cameraMap_;
	Camera* currentCamera_ = nullptr;
}

void CameraManager::Initialize() {
}

Camera* CameraManager::AddCamera(std::string name) {
	Camera* camera = new Camera();
	cameraMap_[name] = camera;
	return camera;
}

Camera* CameraManager::GetCurrentCamera() {
	return currentCamera_;
}

void CameraManager::SetCurrentCamera(std::string name) {
	Camera* camera = GetCamera(name);
	if (camera != nullptr) {
		currentCamera_ = camera;
	}
}

Camera* CameraManager::GetCamera(std::string name) {
	if (cameraMap_.find(name) != cameraMap_.end()) {
		return cameraMap_[name];
	}
    return nullptr;
}
