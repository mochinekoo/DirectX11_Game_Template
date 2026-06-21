#include "FBXModel.h"
#include <fbxsdk.h>
#include "ImGUI/imgui.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include <DirectXMath.h>

using namespace DX3D;
using namespace DirectX;

FBXModel::FBXModel(std::string fileName, XMFLOAT3 loc)
	: BaseObject("FBXModel") {
	fileName_ = fileName;
	location_ = loc;
}

FBXModel::~FBXModel()
{
}

void FBXModel::Initialize() {
	FbxManager* fbxManager = FbxManager::Create();
	FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "FBX");
	fbxImporter->Initialize(fileName_.c_str(), -1, fbxManager->GetIOSettings());
	FbxScene* fbxScene = FbxScene::Create(fbxManager, "FBXScene");
	fbxImporter->Import(fbxScene);
	fbxImporter->Destroy();

	FbxNode* rootNode = fbxScene->GetRootNode();
	for (int i = 0; i < rootNode->GetChildCount(); i++) {
		node_ = rootNode->GetChild(i);
		if (node_->GetMesh() != nullptr) {
			mesh_ = node_->GetMesh();
			break;
		}
	}

	InitVertexBuffer();
	InitIndexBuffer();
}

void FBXModel::InitVertexBuffer() {

	int vertexCount = mesh_->GetControlPointsCount();
	int polygonCount = mesh_->GetPolygonCount();
	int materialCount = node_->GetMaterialCount();
	vertexs_ = new Vertex[vertexCount]();
	
	for (DWORD i = 0; i < polygonCount; i++) {
		for (int v = 0; v < 3; v++) {
			int polyVertexIndex = mesh_->GetPolygonVertex(i, v);
			FbxVector4 loc = mesh_->GetControlPointAt(polyVertexIndex);
			vertexs_[polyVertexIndex].x = (float)loc[0];
			vertexs_[polyVertexIndex].y = (float)loc[1];
			vertexs_[polyVertexIndex].z = (float)loc[2];
		}
	}

	D3D11_BUFFER_DESC desc = {};
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.ByteWidth = sizeof(Vertex) * vertexCount;
	desc.CPUAccessFlags = 0;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);

	D3D11_SUBRESOURCE_DATA vertiesData = {};
	vertiesData.pSysMem = vertexs_;

	GetDevice()->CreateBuffer(&desc, &vertiesData, &vertexsBuffer_);
}

void FBXModel::InitIndexBuffer() {
	int polygonCount = mesh_->GetPolygonCount();
	index_ = new int[polygonCount * 3];

	int count = 0;
	for (DWORD i = 0; i < polygonCount; i++) {
		for (int v = 0; v < 3; v++) {
			index_[count] = mesh_->GetPolygonVertex(i, v);
			count++;
		}
	}

	D3D11_BUFFER_DESC bd = {};
	bd.ByteWidth = sizeof(int) * polygonCount * 3;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = index_;

	GetDevice()->CreateBuffer(&bd, &indexData, &indexBuffer_);
}

void FBXModel::Update() {
	Camera* camera = CameraManager::GetCurrentCamera();
	if (camera == nullptr) return;

	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z);
	XMMATRIX translation = XMMatrixTranslation(location_.x, location_.y, location_.z);
	XMMATRIX world = XMMatrixIdentity();
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixIdentity();

	ConstantBuffer constantBuffer = {};
	constantBuffer.world = XMMatrixTranspose(world);
	constantBuffer.view = XMMatrixTranspose(view);
	constantBuffer.projection = XMMatrixTranspose(projection);

	GetDeviceContext()->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBuffer, 0, 0);
}

void FBXModel::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexsBuffer_, &stride, &offset);
	GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer_);
	GetDeviceContext()->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->IASetInputLayout(ShaderManager::GetVertexShader(TEST_VERTEX_SHADER).inputLayout_);
	GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(TEST_VERTEX_SHADER).vertexShader_, nullptr, 0);
	GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(TEST_PIXEL_SHADER).pixelShader_, nullptr, 0);
	GetDeviceContext()->DrawIndexed((mesh_->GetPolygonCount()) * 3, 0, 0);
	GetDeviceContext()->RSSetState(nullptr);

#ifdef _DEBUG
	ImGui::Begin("FBX");
	ImGui::SliderFloat("LocationX: ", &location_.x, -1280, 1280);
	ImGui::SliderFloat("LocationY: ", &location_.y, -1280, 1280);
	ImGui::SliderFloat("LocationZ: ", &location_.z, -1280, 1280);
	ImGui::SliderFloat("RoationX: ", &rotation_.x, -10.0, 10.0);
	ImGui::SliderFloat("RotationY: ", &rotation_.y, -10.0, 10.0);
	ImGui::SliderFloat("RotationZ: ", &rotation_.z, -10.0, 10.0);
	ImGui::End();
#endif
}

void FBXModel::Release()
{
}