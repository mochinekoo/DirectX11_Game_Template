#include "Cube.h"
#include "CameraManager.h"
#include <DirectXMath.h>
#include "DX3DManager.h"
#include "ShaderManager.h"
#include "LightManager.h"

using namespace DirectX;
using namespace DX3DManager;

namespace {
	const int CUBE_FACE = 6;
}

void Cube::Init() {
	InitVertexBuffer();
	InitIndexBuffer();
	InitConstantBuffer();
	InitMaterial();
}

void Cube::Update() {
	Camera* currentCamera = CameraManager::GetCurrentCamera();
	XMMATRIX world = transform_.GetWorldMatrix();
	XMMATRIX view = currentCamera->GetViewMatrix();
	XMMATRIX projection = currentCamera->GetProjection();

	for (int i = 0; i < CUBE_FACE; i++) {
		ConstantBuffer constantBuffer = {};
		constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
		constantBuffer.worldMatrix_ = XMMatrixTranspose(world);
		constantBuffer.diffuse_ = materialList_[i].diffuse_;
		constantBuffer.ambient_ = materialList_[i].ambient_;
		constantBuffer.specular_ = materialList_[i].specular_;
		constantBuffer.emission_ = materialList_[i].emission_;
		constantBuffer.hasTexture_ = materialList_[i].texture_ == nullptr ? FALSE : TRUE;
		constantBuffer.lightDirection_ = LightManager::GetDefaultLight();
		GetDeviceContext()->UpdateSubresource(constantBufferList_[i], 0, nullptr, &constantBuffer, 0, 0);
	}
}

void Cube::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	ShaderManager::SetPixelShader("MochinekoEngine/PixelShader.hlsl");
	ShaderManager::SetVertexShader("MochinekoEngine/VertexShader.hlsl");

	for (int i = 0; i < CUBE_FACE; i++) {
		ID3D11ShaderResourceView* nullSrv = { nullptr };

		GetDeviceContext()->IASetIndexBuffer(indexBufferList_[i], DXGI_FORMAT_R32_UINT, 0);
		GetDeviceContext()->RSSetState(GetRasterizerState());
		GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBufferList_[i]);
		GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBufferList_[i]);
		GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
		GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		Texture* texture = materialList_[i].texture_;
		if (texture != nullptr) {
			ID3D11ShaderResourceView* textureSrv = texture->GetShaderResourceView();
			ID3D11SamplerState* sampler = texture->GetSamplerState();
			GetDeviceContext()->PSSetShaderResources(0, 1, &textureSrv);
			GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
		}
		else {
			GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
		}
		GetDeviceContext()->DrawIndexed(materialIndexList_[i].size(), 0, 0);
		GetDeviceContext()->RSSetState(nullptr);
	}

}

void Cube::DrawImGUI() {
	//BaseObject::DrawImGUI();

	ImGui::Begin("Cube");
	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("ObjectInfo")) {
			ImGui::SliderFloat("Location X", &transform_.location_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Location Y", &transform_.location_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Location Z", &transform_.location_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity X", &transform_.velocity_.x, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Y", &transform_.velocity_.y, -10.0f, 10.0f);
			ImGui::SliderFloat("Velocity Z", &transform_.velocity_.z, -10.0f, 10.0f);
			ImGui::SliderFloat("Rotation X", &transform_.rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Y", &transform_.rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Rotation Z", &transform_.rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
			ImGui::SliderFloat("Scale X", &transform_.scale_.x, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Y", &transform_.scale_.y, 0.1f, 10.0f);
			ImGui::SliderFloat("Scale Z", &transform_.scale_.z, 0.1f, 10.0f);
			//ImGui::Checkbox("Wireframe", &wireframe_);

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Color")) {
			for (int i = 0; i < CUBE_FACE; i++) {
				Material& material = materialList_[i];
				auto& diffuse = material.diffuse_;
				auto& ambient = material.ambient_;

				float diffuseColor[4] = { diffuse.x, diffuse.y, diffuse.z, diffuse.w };
				ImGui::ColorEdit4((std::string("Diffuse") + std::to_string(i)).c_str(), diffuseColor);
				diffuse = { diffuseColor[0], diffuseColor[1], diffuseColor[2], diffuseColor[3] };
				float ambientColor[4] = { ambient.x, ambient.y, ambient.z, ambient.w };
				ImGui::ColorEdit4((std::string("Ambient") + std::to_string(i)).c_str(), ambientColor);
				ambient = { ambientColor[0], ambientColor[1], ambientColor[2], ambientColor[3] };

			}

			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Texture")) {
			for (int i = 0; i < CUBE_FACE; i++) {
				Material& material = materialList_[i];
				auto texture = material.texture_;
				if (ImGui::Button((std::string("TextureSelect") + std::to_string(i)).c_str())) {
					char fileName[MAX_PATH] = "";
					OPENFILENAMEA openFileName = {};
					openFileName.lStructSize = sizeof(openFileName);
					openFileName.nMaxFile = MAX_PATH;
					openFileName.lpstrFile = fileName;
					openFileName.lpstrFilter = "PNG\0*.png*\0";
					openFileName.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;

					if (GetOpenFileNameA(&openFileName)) {
						material.texture_ = new Texture(fileName);
						material.texture_->Init();
					}
				}
				ImGui::Text("Texture: %s", texture == nullptr ? "(nullptr)" : texture->GetFileName().c_str());
			}

			ImGui::EndTabItem();
		}
	}
	ImGui::EndTabBar();

	ImGui::End();
}

void Cube::Release()
{
}

void Cube::InitVertexBuffer() {
	vertexList_.resize(24);

	XMFLOAT4 color = Color::Black().ConvertXMFLOAT4();
	XMFLOAT3 frontNormal = { 0.0f, 0.0f, -1.0f };
	XMFLOAT3 backNormal = { 0.0f, 0.0f, 1.0f };
	XMFLOAT3 rightNormal = { 1.0f, 0.0f, 0.0f };
	XMFLOAT3 leftNormal = { -1.0f, 0.0f, 0.0f };
	XMFLOAT3 topNormal = { 0.0f, 1.0f, 0.0f };
	XMFLOAT3 bottomNormal = { 0.0f, -1.0f, 0.0f };

	// 手前
	vertexList_[0] = { {0.0f, 0.0f, 0.0f}, frontNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[1] = { {0.0f, 1.0f, 0.0f}, frontNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[2] = { {1.0f, 0.0f, 0.0f}, frontNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[3] = { {1.0f, 1.0f, 0.0f}, frontNormal, color, {0.0f, 0.0f} }; // 右上

	// 奥
	vertexList_[4] = { {0.0f, 0.0f, 1.0f}, backNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[5] = { {0.0f, 1.0f, 1.0f}, backNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[6] = { {1.0f, 0.0f, 1.0f}, backNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[7] = { {1.0f, 1.0f, 1.0f}, backNormal, color, {0.0f, 0.0f} }; // 右上

	// 右
	vertexList_[8] = { {1.0f, 0.0f, 0.0f}, rightNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[9] = { {1.0f, 1.0f, 0.0f}, rightNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[10] = { {1.0f, 0.0f, 1.0f}, rightNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[11] = { {1.0f, 1.0f, 1.0f}, rightNormal, color, {0.0f, 0.0f} }; // 右上

	// 左
	vertexList_[12] = { {0.0f, 0.0f, 1.0f}, leftNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[13] = { {0.0f, 1.0f, 1.0f}, leftNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[14] = { {0.0f, 0.0f, 0.0f}, leftNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[15] = { {0.0f, 1.0f, 0.0f}, leftNormal, color, {0.0f, 0.0f} }; // 右上

	// 上
	vertexList_[16] = { {0.0f, 1.0f, 0.0f}, topNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[17] = { {0.0f, 1.0f, 1.0f}, topNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[18] = { {1.0f, 1.0f, 0.0f}, topNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[19] = { {1.0f, 1.0f, 1.0f}, topNormal, color, {0.0f, 0.0f} }; // 右上

	// 下
	vertexList_[20] = { {0.0f, 0.0f, 1.0f}, bottomNormal, color, {0.0f, 0.0f} }; // 左下
	vertexList_[21] = { {0.0f, 0.0f, 0.0f}, bottomNormal, color, {0.0f, 0.0f} }; // 左上
	vertexList_[22] = { {1.0f, 0.0f, 1.0f}, bottomNormal, color, {0.0f, 0.0f} }; // 右下
	vertexList_[23] = { {1.0f, 0.0f, 0.0f}, bottomNormal, color, {0.0f, 0.0f} }; // 右上

	for (int i = 0; i < CUBE_FACE; i++) {
		int index = i * 4;

		vertexList_[index + 0].uv_ = { 0.0f, 1.0f };
		vertexList_[index + 1].uv_ = { 0.0f, 0.0f };
		vertexList_[index + 2].uv_ = { 1.0f, 1.0f };
		vertexList_[index + 3].uv_ = { 1.0f, 0.0f };
	}


	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexList_.size();
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void Cube::InitConstantBuffer() {
	constantBufferList_.resize(CUBE_FACE);

	for (int i = 0; i < constantBufferList_.size(); i++) {
		D3D11_BUFFER_DESC constantDesc = {};
		constantDesc.Usage = D3D11_USAGE_DEFAULT;
		constantDesc.ByteWidth = sizeof(ConstantBuffer);
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		GetDevice()->CreateBuffer(&constantDesc, nullptr, &constantBufferList_[i]);
	}
}

void Cube::InitIndexBuffer() {
	indexBufferList_.resize(CUBE_FACE);
	materialIndexList_.resize(CUBE_FACE);

	/// 1-------3
	/// |       |
	/// |       |
	/// |       |
	/// 0-------2
	/// 
	/// 
	indexVertexList_ = {
		0, 2, 3, 0, 3, 1,	// 手前
		4, 7, 6, 4, 5, 7,
		8, 10, 11, 8, 11, 9,
		12, 14, 15, 12, 15, 13,
		16, 17, 19, 16, 19, 18,
		20, 21, 23, 20, 23, 22
	};

	for (int i = 0; i < CUBE_FACE; i++) {

		for (int j = 0; j < 6; j++) {
			materialIndexList_[i].push_back(indexVertexList_[i * 6 + j]);
		}

		D3D11_BUFFER_DESC indexBufferDesc = {};
		indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		indexBufferDesc.ByteWidth = sizeof(uint32_t) * materialIndexList_[i].size();
		indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		indexBufferDesc.CPUAccessFlags = 0;
		indexBufferDesc.MiscFlags = 0;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = materialIndexList_[i].data();
		indexData.SysMemPitch = 0;
		indexData.SysMemSlicePitch = 0;
		GetDevice()->CreateBuffer(&indexBufferDesc, &indexData, &indexBufferList_[i]);
	}
}

void Cube::InitMaterial() {
	materialList_.resize(CUBE_FACE);

	for (int i = 0; i < materialList_.size(); i++) {
		Material materialData = {};
		materialData.diffuse_ = Color::Black().ConvertXMFLOAT4();
		materialData.ambient_ = {0.1f, 0.1f, 0.1f, 1.0f};
		materialData.specular_ = {};
		materialData.emission_ = {};
		materialData.shininess_ = {};
		materialData.texture_ = nullptr;

		materialList_[i] = materialData;
	}
}
