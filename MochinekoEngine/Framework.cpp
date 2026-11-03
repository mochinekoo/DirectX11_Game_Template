#include "Framework.h"
#include "ModelManager.h"
#include <functional>
#include "ObjectManager.h"
#include <Windows.h>
#include <string>

void MochinekoEngineResource::Init() {
	sphereColHandle_ = ModelManager::Load("MochinekoEngine/Asset/SphereCollider.fbx");
	boxColHandle_ = ModelManager::Load("MochinekoEngine/Asset/BoxCollider.fbx");
}

void MochinekoUtil::openFileDialog(const char* filter, std::function<void(const std::string& path)> runAction) {
	char fileNameBuffer[MAX_PATH] = {};
	OPENFILENAMEA openFileName = {};
	openFileName.lStructSize = sizeof(OPENFILENAMEA);
	openFileName.hwndOwner = MochinekoEngine::GetGameWindowHandle();
	openFileName.lpstrFilter = filter;
	openFileName.lpstrFile = fileNameBuffer;
	openFileName.nFilterIndex = 1;
	openFileName.nMaxFile = MAX_PATH;
	openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

	if (runAction == NULL) return;
	if (GetOpenFileNameA(&openFileName)) {
		runAction(fileNameBuffer);
	}
}
