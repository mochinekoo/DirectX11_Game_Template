#include "SphereCollider.h"
#include <DirectXMath.h>
#include "../RunningScene.h"
#include "ModelManager.h"
#include "BoxCollider.h"

using namespace DirectX;;
using namespace MochinekoEngineResource;

void SphereCollider::Init() {
	FBX* model = ModelManager::GetModel(sphereColHandle_);
	Transform transform = model->GetTransform();
	transform.scale_ = {radius_, radius_, radius_};
	model->SetTransform(transform);
	model->SetWireframe(true);
}

void SphereCollider::Update() {
	Transform parentTransform = parent_->GetTransform();
    transform_.location_ = parentTransform.location_;
	transform_.scale_ = { radius_, radius_, radius_ };

	FBX* model = ModelManager::GetModel(sphereColHandle_);
	model->SetTransform(transform_);
	model->UpdateTransform();
	model->Update();
}

void SphereCollider::Draw() {
	ModelManager::GetModel(sphereColHandle_)->Draw();
}

bool SphereCollider::IsHitSphereSphere(SphereCollider* col1, SphereCollider* col2) {
    if (col1 != nullptr && col2 != nullptr) {
		XMFLOAT3 aPos = col1->transform_.location_;
		XMFLOAT3 bPos = col2->transform_.location_;
		XMVECTOR vectorA = XMLoadFloat3(&aPos);
		XMVECTOR vectorB = XMLoadFloat3(&bPos);

		XMVECTOR diff = XMVectorSubtract(vectorA, vectorB);
		float distance = XMVectorGetX(XMVector3Length(diff));
		return distance < (col1->GetRadius() + col2->GetRadius());
    }

    return false;
}

bool SphereCollider::IsHitSphereBox(SphereCollider* col1, BoxCollider* col2) {
	if (col1 != nullptr && col2 != nullptr) {
		XMFLOAT3 spherePos = col1->transform_.location_;
		XMFLOAT3 boxPos = col2->GetTransform().location_;
		float sphereRadius = col1->GetRadius();

		XMVECTOR sphereVector = XMLoadFloat3(&spherePos);
		XMVECTOR boxVector = XMLoadFloat3(&boxPos);

		XMFLOAT3 boxSize = col2->GetColliderSize();
		XMVECTOR boxHalfSize = XMVectorSet(boxSize.x / 2.0f, boxSize.y / 2.0f, boxSize.z / 2.0f, 0.0f);

		XMVECTOR minBox = XMVectorSubtract(boxVector, boxHalfSize);
		XMVECTOR maxBox = XMVectorAdd(boxVector, boxHalfSize);
		XMVECTOR closestPoint = XMVectorClamp(sphereVector, minBox, maxBox);
		XMVECTOR diff = XMVectorSubtract(sphereVector, closestPoint);
		float distance = XMVectorGetX(XMVector3LengthSq(diff));
		return distance < (sphereRadius * sphereRadius);
	}
}
