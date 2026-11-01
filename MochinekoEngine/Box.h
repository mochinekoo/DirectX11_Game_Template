#pragma once
#include "BaseObject.h"
#include <vector>
#include <d3d11.h>
#include "DX3DManager.h"
#include <array>

class Box : public BaseObject {
private:
	float width_, height_;

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* constantBuffer_;
	std::vector<Vertex> vertexList_;
	Color color_;
private:
	void InitVertexBuffer();
	void InitConstantBuffer();
public:

	Box(const float width, const float height, const Color color = Color::Black())
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

	float GetWidth() const { return width_; }
	void SetWidth(const float width) { width_ = width; }
	float GetHeight() { return height_; }
	void SetHeight(const float height) { height_ = height; }

	Color GetColor() const { return color_; }
	void SetColor(Color color) { color_ = color; }
	void SetColor(float r, float g, float b, float a) { color_ = {r, g, b, a}; }
	void SetColor(float colorArray[4]) { color_ = {colorArray[0], colorArray[1], colorArray[2], colorArray[3]}; }
};