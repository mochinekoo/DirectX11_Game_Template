#pragma once
#include "BaseObject.h"
#include <vector>
#include <d3d11.h>
#include "DX3DManager.h"
#include <array>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

struct SquareConstantBuffer {
	DirectX::XMMATRIX wvpMatrix_ = {};	// ワールド・ビュー・プロジェクション行列
	DirectX::XMMATRIX worldMatrix_ = {};	// ワールド行列
	DirectX::XMFLOAT4 colorList_[2] = {};
};

class Square : public BaseObject {
private:
	float width_, height_;

	ComPtr<ID3D11Buffer> vertexBuffer_;
	ComPtr<ID3D11Buffer> constantBuffer_;
	std::vector<Vertex> vertexList_;
	std::vector<Color> colorList_;
private:
	void InitVertexBuffer();
	void InitConstantBuffer();
public:

	Square(const float width, const float height, Color color = Color::Black())
		: BaseObject("Square") {
		colorList_.push_back(Color::Blue());
		colorList_.push_back(Color::Red());
		width_ = width;
		height_ = height;
		vertexBuffer_ = nullptr;
		constantBuffer_ = nullptr;
	}

	~Square() {
		Release();
	}

	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

	float GetWidth() const { return width_; }
	void SetWidth(const float width) { width_ = width; }
	float GetHeight() { return height_; }
	void SetHeight(const float height) { height_ = height; }

	auto& GetColorList() const { return colorList_; }
	void SetColor(int index, Color color) { colorList_[index] = color; }
};