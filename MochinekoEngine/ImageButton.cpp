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
		if (hoverFunc != NULL) {
			hoverFunc();
		}
	}
	isPush_ = (isHover_&& InputManager::CheckDownMouse(0));
	if (isPush_) {
		if (pushFunc != NULL) {
			pushFunc();
		}	
	}

	image_->SetTransform(transform_);
	image_->Update();
}

void ImageButton::Draw() {
	image_->Draw();
}

void ImageButton::DrawImGUI() {
	std::string title = GetName() + "(" + GetTag() + ")";
	ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {

			ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 1280.0f);
			ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 720.0f);
			ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 100.0f);
			ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void ImageButton::Release() {
	image_->Release();
}
