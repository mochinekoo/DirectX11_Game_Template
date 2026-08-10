#pragma once
#include <string>
#include <DirectXMath.h>

/// <summary>
/// 画像を管理する名前空間。
/// </summary>
namespace ImageManager {

	void Init();

	int Load(const std::string& fileName);
	void Draw(const int id);
	void SetPostion(const int id, const DirectX::XMFLOAT3 postion);
	void SetVelocity(const int id, const DirectX::XMFLOAT3 velocity);
	void SetRotation(const int id, const DirectX::XMFLOAT3 rote);
	void SetScale(const int id, const DirectX::XMFLOAT3 scale);
}