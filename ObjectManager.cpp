#include "ObjectManager.h"
#include "BaseObject.h"
#include <vector>

namespace {
    std::vector<BaseObject*> objList;
}


void ObjectManager::InitializeObject() {
    for (BaseObject* obj : objList) {
        obj->Initialize();
    }
}

void ObjectManager::AddObject(BaseObject* obj) {
    objList.push_back(obj);
}

void ObjectManager::RemoveObject(BaseObject* obj) {
    for (auto it = objList.begin(); it != objList.end(); ) {
        if ((*it) == obj) {
            delete* it;
            it = objList.erase(it);
        }
        else {
            it++;
        }
    }
}

void ObjectManager::UpdateManager() {
    for (BaseObject* obj : objList) {
        if (obj->IsDead()) {
            RemoveObject(obj);
        }
        else {
            obj->Update();
            obj->Draw();
        }
    }
}

std::vector<BaseObject*>& ObjectManager::GetObjectList() {
    return objList;
}

void ObjectManager::ClearObject() {
    objList.clear();
}
