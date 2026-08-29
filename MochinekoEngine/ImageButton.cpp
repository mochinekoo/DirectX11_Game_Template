#include "ImageButton.h"
#include "InputManager.h"
#include "Transform.h"
#include <DirectXMath.h>

using namespace DirectX;

void ImageButton::Init() {
	image_->Init();
}

void ImageButton::Update() {
	POINT mousePoint = InputManager::GetMousePostion();
	Transform imageTransform = image_->GetTransform();
	XMFLOAT3 imageLoc = imageTransform.location_;

	bool hoverX = imageLoc.x <= mousePoint.x && imageLoc.x + image_->GetWidth() >= mousePoint.x;
	bool hoverY = imageLoc.y <= mousePoint.y && imageLoc.y + image_->GetHeight() >= mousePoint.y;

	isHover_ = (hoverX && hoverY);
	if (isHover_) {
		hoverFunc();
	}
	isPush_ = (isHover_&& InputManager::CheckDownMouse(0));
	if (isPush_) {
		pushFunc();
	}


	image_->Update();
}

void ImageButton::Draw() {
	image_->Draw();
}

void ImageButton::Release() {
	image_->Release();
}
