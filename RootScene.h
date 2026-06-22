#pragma once
#include "BaseScene.h"

class RootScene : public BaseScene {
private:
public:

	RootScene();
	~RootScene();

	void Init() override;
	void Update() override;
	void Draw() override;
	void Release() override;

};

