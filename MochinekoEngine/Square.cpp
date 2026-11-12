#include "Square.h"
#include <d3d11.h>
#include "DX3DManager.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "CameraManager.h"
#include "ImGUI/imgui.h"

using namespace DirectX;
using namespace DX3DManager;

void Square::Init() {
	InitVertexBuffer();
	InitConstantBuffer();
}

void Square::Update() {
	Camera* currentCamera = CameraManager::GetCurrentCamera();
	XMMATRIX size = XMMatrixScaling(width_, height_, 1.0f);
	XMMATRIX world = size * transform_.GetWorldMatrix();
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f,
		720.0f, 0.0f,
		0.0f, 100.0f
	);

	SphereConstantBuffer constantBuffer = {};
	constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
	constantBuffer.worldMatrix_ = XMMatrixTranspose(world);
	for (int i = 0; i < colorList_.size(); i++) {
		Color& color = colorList_[i];
		constantBuffer.colorList_[i] = colorList_[i].ConvertXMFLOAT4();
	}
	GetDeviceContext()->UpdateSubresource(constantBuffer_.Get(), 0, nullptr, &constantBuffer, 0, 0);
}

void Square::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("MochinekoEngine/SpherePixelShader.hlsl");
	ShaderManager::SetVertexShader("MochinekoEngine/SphereVertexShader.hlsl");

	ID3D11ShaderResourceView* nullSrv = { nullptr };
	GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
	GetDeviceContext()->IASetVertexBuffers(0, 1, vertexBuffer_.GetAddressOf(), &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->VSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());
	GetDeviceContext()->PSSetConstantBuffers(0, 1, constantBuffer_.GetAddressOf());

	GetDeviceContext()->Draw(6, 0);

	GetDeviceContext()->RSSetState(nullptr);
}

void Square::DrawImGUI() {
	std::string title = GetName() + "(" + GetTag() + ")";
	ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoDocking);
	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {

			ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 1280.0f);
			ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 720.0f);
			ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 100.0f);
			ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);

			ImGui::SliderFloat("Width", &width_, 0.0f, 100.0f);
			ImGui::SliderFloat("Height", &height_, 0.0f, 100.0f);
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Color")) {
			for (int i = 0; i < colorList_.size(); i++) {
				Color& color = colorList_[i];
				float colorList[4] = {color.r_, color.g_, color.b_, color.a_};
				ImGui::ColorPicker4(std::to_string(i).c_str(), colorList);
				SetColor(i, Color(colorList[0], colorList[1], colorList[2], colorList[3]));
			}

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void Square::Release()
{
}


void Square::InitVertexBuffer() {
	vertexList_.resize(6);

	//XMFLOAT4 color = color_.ConvertXMFLOAT4();
	auto color = colorList_[0].ConvertXMFLOAT4();

	vertexList_[0] = { {0.0f, 0.0f, 0.0f}, {}, color, {0.0f, 0.0f}};	// 左上
	vertexList_[1] = { {0.0f, 1.0f, 0.0f}, {}, color, {0.0f, 1.0f} };	// 左下
	vertexList_[2] = { { 1.0f, 0.0f, 0.0f}, {}, color, {1.0f, 0.0f} };	// 右上

	vertexList_[3] = { { 1.0f, 0.0f, 0.0f}, {}, color, {1.0f, 0.0f} };	// 右上
	vertexList_[4] = { { 1.0f,  1.0f, 0.0f}, {}, color, {1.0f, 1.0f} };	// 右下
	vertexList_[5] = { {0.0f,  1.0f, 0.0f}, {}, color, {0.0f, 1.0f} };	// 左下

	/*
	vertexList_[0] = { {0.0f, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[1] = { {0.0f, (float) height_, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[2] = { { (float)width_, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[3] = { { (float)width_, 0.0f, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[4] = { { (float) width_,  (float)height_, 0.0f}, {}, color_, {0.0f, 0.0f} };
	vertexList_[5] = { {0.0f,  (float)height_, 0.0f}, {}, color_, {0.0f, 0.0f} };
	*/

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexList_.size();
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void Square::InitConstantBuffer() {
	D3D11_BUFFER_DESC constantDesc = {};
	constantDesc.Usage = D3D11_USAGE_DEFAULT;
	constantDesc.ByteWidth = sizeof(SphereConstantBuffer);
	constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	GetDevice()->CreateBuffer(&constantDesc, nullptr, &constantBuffer_);
}