#pragma once
#include "BaseObject.h"
#include <vector>

namespace ObjectManager {

	void InitializeObject();
	void AddObject(BaseObject* obj);
	void RemoveObject(BaseObject* obj);
	void UpdateManager();
	std::vector<BaseObject*>& GetObjectList();
	void ClearObject();
	
	template<typename T> T* FindObject() {
		for (int n = 0; n < GetObjectList().size(); n++) {
			BaseObject* obj = GetObjectList()[n];
			T* instance = dynamic_cast<T*>(obj);
			if (instance != nullptr) {
				return instance;
			}
		}
		return nullptr;
	}

	template<typename T> std::vector<T*> FindObjectList() {
		std::vector<T*> objGetList;
		for (int n = 0; n < GetObjectList().size(); n++) {
			BaseObject* obj = GetObjectList()[n];
			T* instance = dynamic_cast<T*>(obj);
			if (instance != nullptr) {
				objGetList.push_back(instance);
			}
		}
		return objGetList;
	}
}