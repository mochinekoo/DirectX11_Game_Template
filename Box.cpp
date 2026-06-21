#include "Box.h"
#include "DX3D.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "ImGUI/imgui.h"

using namespace DirectX;
using namespace DX3D;

Box::Box(XMFLOAT3 location)
	: BaseObject("Box") {
	location_ = location;

	vertices_[0] = { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	vertices_[1] = { 0.0f, -64.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	vertices_[2] = { 64.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };

	vertices_[3] = { 64.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f };
	vertices_[4] = { 64.0f, -64.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 1.0f };
	vertices_[5] = { 0.0f, -64.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
}

Box::~Box() {
}

void Box::Initialize() {
	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(vertices_);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA verticeData = {};
	verticeData.pSysMem = vertices_;
	
	GetDevice()->CreateBuffer(&desc, &verticeData, &vertexBuffer_);

	D3D11_BUFFER_DESC constantBufferDesc = {};
	constantBufferDesc.ByteWidth = sizeof(ConstantBuffer);
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantBufferDesc, nullptr, &constantBuffer_);
}

void Box::Update() {
	Camera* camera = CameraManager::GetCurrentCamera();
	if (camera == nullptr) return;

	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z);
	XMMATRIX translation = XMMatrixTranslation(location_.x, location_.y, location_.z);
	XMMATRIX world = scale * rotation * translation;
	XMMATRIX view = camera->GetViewMatrix();
	XMMATRIX projection = camera->projection_;

	ConstantBuffer constantBuffer = {};
	constantBuffer.world = XMMatrixTranspose(world);
	constantBuffer.view = XMMatrixTranspose(view);
	constantBuffer.projection = XMMatrixTranspose(projection);

	GetDeviceContext()->UpdateSubresource(constantBuffer_, 0, nullptr, &constantBuffer, 0, 0);
}

void Box::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer_);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->IASetInputLayout(ShaderManager::GetVertexShader(TEST_VERTEX_SHADER).inputLayout_);
	GetDeviceContext()->VSSetShader(ShaderManager::GetVertexShader(TEST_VERTEX_SHADER).vertexShader_, nullptr, 0);
	GetDeviceContext()->PSSetShader(ShaderManager::GetPixelShader(TEST_PIXEL_SHADER).pixelShader_, nullptr, 0);
	GetDeviceContext()->Draw(6, 0);
	GetDeviceContext()->RSSetState(nullptr);

#ifdef _DEBUG
	ImGui::Begin("Box");
	ImGui::SliderFloat("LocationX: ", &location_.x, -1280, 1280);
	ImGui::SliderFloat("LocationY: ", &location_.y, -1280, 1280);
	ImGui::SliderFloat("LocationZ: ", &location_.z, -1280, 1280);
	ImGui::SliderFloat("RoationX: ", &rotation_.x, -10.0, 10.0);
	ImGui::SliderFloat("RotationY: ", &rotation_.y, -10.0, 10.0);
	ImGui::SliderFloat("RotationZ: ", &rotation_.z, -10.0, 10.0);
	ImGui::End();
#endif
}

void Box::Release()
{
}
