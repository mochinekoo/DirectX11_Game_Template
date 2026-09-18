#include "ModelManager.h"
#include "FBX.h"
#include <vector>

namespace {
    std::vector<FBX*> modelList_;
}

void ModelManager::Init() {
    modelList_.clear();
}

int ModelManager::Load(const std::string& fileName) {
    for (int i = 0; i < modelList_.size(); i++) {
        FBX* fbx = modelList_[i];
        if (fbx->GetFileName() == fileName) {
            return i;
        }
    }

    FBX* fbx = new FBX(fileName);
    fbx->Init();
    modelList_.push_back(fbx);
    return modelList_.size() - 1;
}

void ModelManager::Draw(int id) {
    modelList_[id]->Draw();
}

Transform ModelManager::GetTransform(const int id) {
    return modelList_[id]->GetTransform();
}

void ModelManager::SetTransform(const int id, const Transform& transform) {
    modelList_[id]->SetTransform(transform);
}

FBX* ModelManager::GetModel(const int id) {
    return modelList_[id];
}
