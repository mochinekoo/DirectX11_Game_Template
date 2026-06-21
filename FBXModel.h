#pragma once
#include "BaseObject.h"
#include <string>
#include <fbxsdk.h>
#include "DX3D.h"

#pragma comment(lib, "LibFbxSDK-MD.lib")
#pragma comment(lib, "LibXml2-MD.lib")
#pragma comment(lib, "zlib-MD.lib")

class FBXModel : public BaseObject {
private:
	std::string  fileName_ = "";
	Vertex* vertexs_ = nullptr;
	ID3D11Buffer* vertexsBuffer_ = nullptr;
	ID3D11Buffer* indexBuffer_ = nullptr;
	ID3D11Buffer* constantBuffer_ = nullptr;
	int* index_ = nullptr;

	fbxsdk::FbxMesh* mesh_ = nullptr;
	fbxsdk::FbxNode* node_ = nullptr;
public:

	FBXModel(std::string fileName, DirectX::XMFLOAT3 loc);
	~FBXModel();

	void Initialize() override;
	void InitVertexBuffer();
	void InitIndexBuffer();
	void Update() override;
	void Draw() override;
	void Release() override;

};