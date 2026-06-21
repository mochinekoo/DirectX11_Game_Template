#pragma once
#include <DirectXMath.h>
#include <string>

struct Camera {
	std::string name_ = {};
	DirectX::XMFLOAT3 location_ = {};
	DirectX::XMFLOAT3 target_ = {};
	DirectX::XMFLOAT3 upDirection_ = {};
	DirectX::XMMATRIX projection_ = {};

	DirectX::XMMATRIX GetViewMatrix() {
		DirectX::XMVECTOR locVec = DirectX::XMVectorSet(location_.x, location_.y, location_.z, 1.0f);
		DirectX::XMVECTOR targetVec = DirectX::XMVectorSet(target_.x, target_.y, target_.z, 1.0f);
		DirectX::XMVECTOR upDirVec = DirectX::XMVectorSet(upDirection_.x, upDirection_.y, upDirection_.z, 0.0f);
		DirectX::XMVECTOR directionVec = DirectX::XMVectorSubtract(targetVec, locVec);
		if (DirectX::XMVector3Equal(directionVec, DirectX::XMVectorZero())) {
			targetVec = DirectX::XMVectorAdd(targetVec, DirectX::XMVectorSet(1.0f, 1.0f, 1.0f, 0.0f));
		}
		if (DirectX::XMVector3Equal(upDirVec, DirectX::XMVectorZero())) {
			upDirVec = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		}

		return DirectX::XMMatrixLookAtLH(locVec, targetVec, upDirVec);
	}

};

namespace CameraManager {
	void Initialize();
	Camera* AddCamera(std::string name);
	Camera* GetCurrentCamera();
	void SetCurrentCamera(std::string name);
	Camera* GetCamera(std::string name);
}