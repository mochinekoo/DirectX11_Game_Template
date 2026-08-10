#include "Image.h"
#include "ShaderManager.h"
#include "DX3DManager.h"
#include <DirectXMath.h>
#include "ImGUI/imgui.h"

using namespace DX3DManager;
using namespace DirectX;

void Image::Init() {
	texture_->Init();
}

void Image::Update() {
	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z);
	XMMATRIX translation = XMMatrixTranslation(location_.x, location_.y, location_.z);
	XMMATRIX world = scale * rotation * translation;
	XMMATRIX view = XMMatrixIdentity();
	XMMATRIX projection = XMMatrixOrthographicOffCenterLH(
		0.0f, 1280.0f,
		720.0f, 0.0f,
		0.0f, 100.0f
	);

	ConstantBuffer constantBuffer = {};
	constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
	constantBuffer.diffuse_ = {};
	constantBuffer.ambient_ = {};
	constantBuffer.hasTexture_ = TRUE;
	GetDeviceContext()->UpdateSubresource(texture_->GetConstanctBuffer(), 0, nullptr, &constantBuffer, 0, 0);
}

void Image::Draw() {
	auto vertexBuffer = texture_->GetVertexBuffer();
	auto constantBuffer = texture_->GetConstanctBuffer();
	auto srv = texture_->GetShaderResourceView();
	auto samplerState = texture_->GetSamplerState();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("PixelShader.hlsl");
	ShaderManager::SetVertexShader("VertexShader.hlsl");
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterizerState());
	GetDeviceContext()->PSSetShaderResources(0, 1, &srv);
	GetDeviceContext()->PSSetSamplers(0, 1, &samplerState);

	GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBuffer);
	GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBuffer);

	GetDeviceContext()->Draw(6, 0);

	std::string title = GetName() + "(" + GetTag() + ")";
	ImGui::Begin(title.c_str(), nullptr, ImGuiWindowFlags_NoDocking);

	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {
			ImGui::SliderFloat("Location X", &location_.x, -10.0f, 1280.0f);
			ImGui::SliderFloat("Location Y", &location_.y, -10.0f, 720.0f);
			ImGui::SliderFloat("Location Z", &location_.z, -10.0f, 100.0f);
			ImGui::SliderFloat("Velocity X", &velocity_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Y", &velocity_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Z", &velocity_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation X", &rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Y", &rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Z", &rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Scale X", &scale_.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &scale_.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &scale_.z, 0.1f, 10.0f);

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();



	GetDeviceContext()->RSSetState(nullptr);
}

void Image::Release() {
}
