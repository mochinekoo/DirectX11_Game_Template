#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"

void DebugScene::Init() {
	ObjectManager::AddObject(new FBX("anime.fbx"));
}

void DebugScene::Update() {
}

void DebugScene::Draw() {

}
