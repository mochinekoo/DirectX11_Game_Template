#include "FBX.h"
#include <vector>
#include "fbxsdk.h"
#include "DX3DManager.h"
#include "ImGUI/imgui.h"

using namespace fbxsdk;
using namespace DX3DManager;
using namespace DirectX;

void FBX::Init() {
	InitFBX();
	InitVertexBuffer();
	InitIndexBuffer();
	InitConstantBuffer();
	InitMaterial();
	InitBone();
} 

void FBX::Update() {
	XMMATRIX scale = XMMatrixScaling(scale_.x, scale_.y, scale_.z);
	XMMATRIX rotation = XMMatrixRotationRollPitchYaw(rotation_.x, rotation_.y, rotation_.z);
	XMMATRIX translation = XMMatrixTranslation(location_.x, location_.y, location_.z);
	XMMATRIX world = scale * rotation * translation;
	XMVECTOR eye = XMVectorSet(0.0f, 0.0f, -5.0f, 0.0f);
	XMVECTOR at = XMVectorSet(0.0f, 2.0f, 0.0f, 0.0f);
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMMATRIX view = XMMatrixLookAtLH(eye, at, up);
	XMMATRIX projection = XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(60.0f), 1280.0f / 720.0f, 0.1f, 100.0f);

	for (int i = 0; i < materialCount_; i++) {
		ConstantBuffer constantBuffer = {};
		constantBuffer.wvpMatrix_ = XMMatrixTranspose(world * view * projection);
		constantBuffer.diffuse_ = materialList_[i].diffuse_;
		constantBuffer.ambient_ = materialList_[i].ambient_;
		constantBuffer.hasTexture_ = (materialList_[i].texture_ != nullptr);
		GetDeviceContext()->UpdateSubresource(constantBufferList_[i], 0, nullptr, &constantBuffer, 0, 0);
	}
}

void FBX::Draw() {
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	

	GetDeviceContext()->PSSetShader(pixelShader_, nullptr, 0);
	GetDeviceContext()->VSSetShader(vertexShader_, nullptr, 0);
	GetDeviceContext()->IASetInputLayout(inputLayout_);
	GetDeviceContext()->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	GetDeviceContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	GetDeviceContext()->RSSetState(GetRasterizerState());

	for (int i = 0; i < materialCount_; i++) {
		GetDeviceContext()->IASetIndexBuffer(indexBuffer_[i], DXGI_FORMAT_R32_UINT, 0);
		GetDeviceContext()->VSSetConstantBuffers(0, 1, &constantBufferList_[i]);
		GetDeviceContext()->PSSetConstantBuffers(0, 1, &constantBufferList_[i]);

		Texture* texture = materialList_[i].texture_;
		if (texture != nullptr) {
			ID3D11ShaderResourceView* textureSrv = texture->GetShaderResourceView();
			ID3D11SamplerState* sampler = texture->GetSamplerState();
			GetDeviceContext()->PSSetShaderResources(0, 1, &textureSrv);
			GetDeviceContext()->PSSetSamplers(0, 1, &sampler);
		}
		else {
			ID3D11ShaderResourceView* nullSrv = { nullptr };
			GetDeviceContext()->PSSetShaderResources(0, 1, &nullSrv);
		}
		GetDeviceContext()->DrawIndexed(materialIndexList_[i].size(), 0, 0);
	}

	GetDeviceContext()->RSSetState(nullptr);

	std::string title = GetName() + "(" + GetTag() + ")";
	ImGui::Begin(title.c_str());
	ImGui::SliderFloat("Location X", &location_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("Location Y", &location_.y, -10.0f, 10.0f);
	ImGui::SliderFloat("Location Z", &location_.z, -10.0f, 10.0f);
	ImGui::SliderFloat("Velocity X", &velocity_.x, -10.0f, 10.0f);
	ImGui::SliderFloat("Velocity Y", &velocity_.y, -10.0f, 10.0f);
	ImGui::SliderFloat("Velocity Z", &velocity_.z, -10.0f, 10.0f);
	ImGui::SliderFloat("Rotation X", &rotation_.x, -DirectX::XM_PI, DirectX::XM_PI);
	ImGui::SliderFloat("Rotation Y", &rotation_.y, -DirectX::XM_PI, DirectX::XM_PI);
	ImGui::SliderFloat("Rotation Z", &rotation_.z, -DirectX::XM_PI, DirectX::XM_PI);
	ImGui::SliderFloat("Scale X", &scale_.x, 0.1f, 10.0f);
	ImGui::SliderFloat("Scale Y", &scale_.y, 0.1f, 10.0f);
	ImGui::SliderFloat("Scale Z", &scale_.z, 0.1f, 10.0f);
	ImGui::End();

}

void FBX::Release() {
}

void FBX::InitFBX() {
	 FbxManager* fbxManager = FbxManager::Create();
	 FbxIOSettings* ios = FbxIOSettings::Create(fbxManager, IOSROOT);
	 fbxManager->SetIOSettings(ios);
	 FbxImporter* fbxImporter = FbxImporter::Create(fbxManager, "fbximporter");
	 fbxImporter->Initialize(fileName_.c_str(), -1, fbxManager->GetIOSettings());
	 FbxScene* fbxScene = FbxScene::Create(fbxManager, "fbxscene");
	 fbxImporter->Import(fbxScene);

	 FbxGeometryConverter converter(fbxManager);
	 converter.Triangulate(fbxScene, true);

	 FbxNode* rootNode = fbxScene->GetRootNode();
	 int childCount = rootNode->GetChildCount();

	 for (int i = 0; i < childCount; i++) {
		 FbxNode* childNode = rootNode->GetChild(i);
		 if (childNode->GetMesh() == nullptr) continue;
		 node_ = childNode;
		 mesh_ = childNode->GetMesh();
		 break;
	 }

	 polygonCount_ = mesh_->GetPolygonCount();
	 materialCount_ = node_->GetMaterialCount();
}

void FBX::InitVertexBuffer() {
	assert(mesh_ != nullptr);

	FbxLayer* layer = mesh_->GetLayer(0);
	FbxLayerElementMaterial* materialList = layer->GetMaterials();
	FbxLayerElement::EMappingMode mapping = materialList->GetMappingMode();

	for (DWORD polyCount = 0; polyCount < polygonCount_; polyCount++) {
		for (int vertexCount = 0; vertexCount < 3; vertexCount++) {
			int vertexIndex = mesh_->GetPolygonVertex(polyCount, vertexCount);
			FbxVector4 vertexLoc = mesh_->GetControlPointAt(vertexIndex);
			int uvIndex = mesh_->GetTextureUVIndex(polyCount, vertexCount);
			FbxLayerElementUV* uvLayer = mesh_->GetLayer(0)->GetUVs();
			FbxVector2 uvLoc = uvLayer->GetDirectArray().GetAt(uvIndex);
			Vertex vertex = {};
			vertex.location_ = {(float) vertexLoc[0], (float)vertexLoc[2], (float)vertexLoc[1]};
			vertex.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
			vertex.uv_ = { (float)uvLoc.mData[0], (float)(1.0f - uvLoc.mData[1])};
			vertexList_.push_back(vertex);
		}
	}
	vertexCount_ = vertexList_.size();

	D3D11_BUFFER_DESC vertexDesc = {};
	vertexDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexDesc.ByteWidth = sizeof(Vertex) * vertexCount_;
	vertexDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = vertexList_.data();

	GetDevice()->CreateBuffer(&vertexDesc, &vertexData, &vertexBuffer_);
}

void FBX::InitIndexBuffer() {
	indexBuffer_.resize(materialCount_);
	materialIndexList_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++) {
		for (int polyCount = 0; polyCount < polygonCount_; polyCount++) {
			FbxLayerElementMaterial* materialList = mesh_->GetLayer(0)->GetMaterials();
			int materialIndex = materialList->GetIndexArray().GetAt(polyCount);
			if (materialIndex == i) {
				for (int vertexCount = 0; vertexCount < 3; vertexCount++) {
					materialIndexList_[i].push_back(polyCount * 3 + vertexCount);
				}
			}
		}

		D3D11_BUFFER_DESC indexDesc = {};
		indexDesc.Usage = D3D11_USAGE_DEFAULT;
		indexDesc.ByteWidth = sizeof(int) * materialIndexList_[i].size();
		indexDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

		D3D11_SUBRESOURCE_DATA indexData = {};
		indexData.pSysMem = materialIndexList_[i].data();

		GetDevice()->CreateBuffer(&indexDesc, &indexData, &indexBuffer_[i]);
	}
}

void FBX::InitConstantBuffer() {
	constantBufferList_.resize(materialCount_);
	for (int i = 0; i < materialCount_; i++) {
		D3D11_BUFFER_DESC constantDesc = {};
		constantDesc.Usage = D3D11_USAGE_DEFAULT;
		constantDesc.ByteWidth = sizeof(ConstantBuffer);
		constantDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		GetDevice()->CreateBuffer(&constantDesc, nullptr, &constantBufferList_[i]);
	}
}

void FBX::InitMaterial() {
	materialList_.resize(materialCount_);

	for (int i = 0; i < materialCount_; i++) {
		FbxSurfaceMaterial* material = node_->GetMaterial(i);
		FbxProperty property = material->FindProperty(FbxSurfaceMaterial::sDiffuse);
		int textureCount = property.GetSrcObjectCount<FbxFileTexture>();
		FbxSurfaceLambert* lambartMaterial = (FbxSurfaceLambert*)node_->GetMaterial(i);
		FbxDouble3 diffuse = lambartMaterial->Diffuse;
		FbxDouble3 ambient = lambartMaterial->Ambient;
		
		Material materialData = {};
		materialData.diffuse_ = { (float)diffuse[0], (float)diffuse[1], (float)diffuse[2], 1.0f };
		materialData.ambient_ = { (float)ambient[0], (float)ambient[1], (float)ambient[2], 1.0f };

		if (textureCount > 0) {
			FbxFileTexture* fileTexture = property.GetSrcObject<FbxFileTexture>();
			Texture* texture = new Texture(fileTexture->GetFileName());
			texture->Init();
			materialData.texture_ = texture;
		}

		materialList_[i] = materialData;
	}
}

void FBX::InitBone() {
	FbxDeformer* deformer = mesh_->GetDeformer(0, FbxDeformer::eSkin);
	skin_ = (FbxSkin*)deformer;
}
