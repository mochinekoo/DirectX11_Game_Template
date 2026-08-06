#pragma once
#include <vector>
#include "BaseObject.h"

namespace ObjectManager {

	void Init();
	void Update();

	BaseObject* AddObject(BaseObject* object);
	void RemoveObject(BaseObject* object);
	std::vector<BaseObject*>& GetAllObject();

	template<typename T> T* FindObject() {
		auto& allObj = GetAllObject();
		for (int i = 0; i < allObj.size(); i++) {
			BaseObject* object = allObj[i];
			if (object == nullptr) continue;
			T* castObject = dynamic_cast<T*>(object);
			if (castObject != nullptr) {
				return castObject;
			}
		}
	}

	template<typename T> std::vector<T*> FindObjects() {
		std::vector<T*> findObjects;
		auto& allObj = GetAllObject();
		for (int i = 0; i < allObj.size(); i++) {
			BaseObject* object = allObj[i];
			if (object == nullptr) continue;
			T* castObject = dynamic_cast<T*>(object);
			if (castObject != nullptr) {
				findObjects.push_back(castObject);
			}
		}
		return findObjects;
	}

}