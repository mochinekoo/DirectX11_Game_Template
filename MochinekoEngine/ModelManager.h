#pragma once
#include <string>
#include "Transform.h"
#include "FBX.h"

namespace ModelManager {

	void Init();
	int Load(const std::string& fileName);
	void Draw(int id);
	Transform GetTransform(const int id);
	void SetTransform(const int id, const Transform& transform);
	FBX* GetModel(const int id);

}