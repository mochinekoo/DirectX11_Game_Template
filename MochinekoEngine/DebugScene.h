#pragma once
#include "BaseScene.h"
class DebugScene : public BaseScene {
private:
	int counter_;
	int imageHandle_;
public:

	DebugScene()
		: BaseScene("DebugScene") {
		counter_ = 0;
		imageHandle_ = -1;
	};

	~DebugScene() {
	};

	void Init() override;
	void Update() override;
	void Draw() override;

};

