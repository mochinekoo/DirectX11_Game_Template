#include "Framework.h"
#include "ModelManager.h"

void MochinekoEngineResource::Init() {
	sphereColHandle_ = ModelManager::Load("MochinekoEngine/Asset/SphereCollider.fbx");
	boxColHandle_ = ModelManager::Load("MochinekoEngine/Asset/BoxCollider.fbx");
}
