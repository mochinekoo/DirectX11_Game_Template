#include "DebugScene.h"
#include "ObjectManager.h"
#include "FBX.h"
#include "SoundManager.h"
#include "Image.h"

void DebugScene::Init() {
	ObjectManager::AddObject(new FBX("anime.fbx"));
	SoundManager::Load("Bossa_Latte.mp3");
	SoundManager::Play("Bossa_Latte.mp3");
	ObjectManager::AddObject(new Image("cap.png"));
}

void DebugScene::Update() {
}

void DebugScene::Draw() {

}
