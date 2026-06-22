#include "RootScene.h"
#include "ObjectManager.h"
#include "FBXModel.h"
#include "Box.h"

RootScene::RootScene()
	: BaseScene("RootScene") {
}

RootScene::~RootScene()
{
}

void RootScene::Init() {
	ObjectManager::AddObject(new FBXModel("Assets/TestModel.fbx", { 100.0f, 100.0f, 0.0f }));
	ObjectManager::AddObject(new Box({ 100.0f, 100.0f, 0.0f }));
}

void RootScene::Update()
{
}

void RootScene::Draw()
{
}

void RootScene::Release()
{
}
