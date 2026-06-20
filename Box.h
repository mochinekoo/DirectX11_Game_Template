#pragma once
#include "BaseObject.h"
#include <d3d11.h>
#include "DX3D.h"

class Box : public BaseObject {
private:
	ID3D11Buffer* vertexBuffer_ = nullptr;
	ID3D11Buffer* constantBuffer_ = nullptr;
	Vertex vertices_[6] = {};
public:

	Box(DirectX::XMFLOAT3 location);
	~Box();

	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Release() override;


};

