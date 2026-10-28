#pragma once
#include "BaseObject.h"
#include <vector>
#include <d3d11.h>
#include "DX3DManager.h"

class Box : public BaseObject {
private:
	float width_, height_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* constantBuffer_;
	std::vector<Vertex> vertexList_;
	DirectX::XMFLOAT4 color_;
private:
	void InitVertexBuffer();
	void InitConstantBuffer();
public:

	Box(const float width, const float height, const DirectX::XMFLOAT4& color = {0.0f, 0.0f, 0.0f, 1.0f})
		: BaseObject("Box") {
		color_ = color;
		width_ = width;
		height_ = height;
		vertexBuffer_ = nullptr;
		constantBuffer_ = nullptr;
	}

	~Box() {}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	float GetWidth() {
		return width_;
	}

	void SetWidth(const float width) {
		width_ = width;
	}

	float GetHeight() {
		return height_;
	}

	void SetHeight(const float height) {
		height_ = height;
	}
};