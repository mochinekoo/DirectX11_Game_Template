#pragma once
#include "BaseObject.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "DX3DManager.h"
#include "FBX.h"
#include <vector>

class Cube : public BaseObject {
private:
	DirectX::XMFLOAT3 cubeSize_;
	ID3D11Buffer* vertexBuffer_;
	// ID3D11Buffer* constantBuffer_;
	std::vector<ID3D11Buffer*> constantBufferList_;
	std::vector<Vertex> vertexList_;

	std::vector<int> indexVertexList_;
	// ID3D11Buffer* indexBuffer_;
	
	std::vector<ID3D11Buffer*> indexBufferList_;
	std::vector<std::vector<int>> materialIndexList_;
	std::vector<Material> materialList_;
private:
	void InitVertexBuffer();
	void InitConstantBuffer();
	void InitIndexBuffer();
	void InitMaterial();
public:

	Cube(DirectX::XMFLOAT3 cubeSize)
		: BaseObject("Cube") {
		cubeSize_ = cubeSize;
	};
	~Cube() {

	};
	
	void Init() override;
	void Update() override;
	void Draw() override;
	void DrawImGUI() override;
	void Release() override;

};