#include "DX3DManager.h"
#include <Windows.h>
#include <d3dcompiler.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "Framework.h"
#include "ShaderManager.h"
#include <wrl/client.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace MochinekoEngine;
using Microsoft::WRL::ComPtr;

namespace DX3DManager {
	inline ComPtr<ID3D11Device> device_ = nullptr;
	inline ComPtr<ID3D11DeviceContext> deviceContext_ = nullptr;
	inline ComPtr<IDXGISwapChain> swapChain_ = nullptr;
	inline ComPtr<ID3D11RenderTargetView> renderTargetView_ = nullptr;
	inline ComPtr<ID3D11Texture2D> texture2D_ = nullptr;
	inline ComPtr<ID3D11Texture2D> depthStencilTexture_ = nullptr;
	inline ComPtr<ID3D11DepthStencilState> depthState_ = nullptr;
	inline ComPtr<ID3D11DepthStencilView> depthStencilView_ = nullptr;
	inline ComPtr<ID3D11RasterizerState> rasterizerState_ = nullptr;
	inline ComPtr<ID3D11RasterizerState> wireframeRasterizerState_ = nullptr;
	inline ComPtr<ID3D11BlendState> blendState_ = nullptr;
}

void DX3DManager::InitDX3D() {
	InitDevice();
	InitDepthBuffer();
	InitViewPort();
	InitRasterizer();
	InitBlend();
	InitShader();
}

void DX3DManager::InitShader() {
	ShaderManager::Init();
	ShaderManager::AddShader(ShaderType::PIXEL_SHADER, "MochinekoEngine/PixelShader.hlsl");
	ShaderManager::AddShader(ShaderType::VERTEX_SHADER, "MochinekoEngine/VertexShader.hlsl");
}

void DX3DManager::InitDevice() {
	HRESULT resultFunc = S_OK;

	///
	/// スワップチェイン
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/dxgi/ns-dxgi-dxgi_swap_chain_desc
	/// 
	DXGI_SWAP_CHAIN_DESC swapchainDesc = {};
	swapchainDesc.BufferDesc.Width = 1280;
	swapchainDesc.BufferDesc.Height = 720;
	swapchainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapchainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapchainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapchainDesc.Windowed = TRUE;
	swapchainDesc.OutputWindow = GetGameWindowHandle();
	swapchainDesc.BufferCount = 1;
	swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapchainDesc.SampleDesc.Count = 1;	
	swapchainDesc.SampleDesc.Quality = 0;	

	UINT createFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
	createFlags |= D3D11_CREATE_DEVICE_DEBUG;
	D3D_FEATURE_LEVEL level = {};
	resultFunc = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		createFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,		// DirectXのバージョン
		&swapchainDesc,			// スワップチェインの設定
		&swapChain_,			// スワップチェインそのもの
		&device_,				// デバイス
		&level,					// 機能レベル
		&deviceContext_			// デバイスコンテキスト
	);

	assert(SUCCEEDED(resultFunc));

	resultFunc = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)texture2D_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));
	resultFunc = device_->CreateRenderTargetView(texture2D_.Get(), NULL, renderTargetView_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));
}

void DX3DManager::InitDepthBuffer() {
	HRESULT resultFunc = S_OK;

	///
	/// 2Dテクスチャ
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_texture2d_desc
	/// 
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 1280;								// テクスチャの幅
	depthDesc.Height = 720;								// テクスチャの高さ
	depthDesc.MipLevels = 1;							// 遠くにある物体などを縮小するレベル
	depthDesc.ArraySize = 1;							// テクスチャ配列内のテクスチャ数
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;						
	depthDesc.SampleDesc.Quality = 0;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;

	resultFunc = device_->CreateTexture2D(&depthDesc, nullptr, depthStencilTexture_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));

	///
	/// 深度ステンシルビュー
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_view_desc
	/// 
	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	resultFunc = device_->CreateDepthStencilView(depthStencilTexture_.Get(), &depthStencilViewDesc, depthStencilView_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));
}

void DX3DManager::InitViewPort() {
	HRESULT resultFunc = S_OK;

	///
	/// ビューポート
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_viewport
	/// 
	D3D11_VIEWPORT viewport = {};
	viewport.Width = 1280;
	viewport.Height = 720;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	deviceContext_->RSSetViewports(1, &viewport);
}

void DX3DManager::InitRasterizer() {
	HRESULT resultFunc = S_OK;

	///
	/// ラスタライザー
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc
	/// 
	D3D11_RASTERIZER_DESC rasterizerDesc = {};
	rasterizerDesc.FillMode = D3D11_FILL_SOLID;
	rasterizerDesc.CullMode = D3D11_CULL_NONE;
	rasterizerDesc.FrontCounterClockwise = FALSE;

	D3D11_RASTERIZER_DESC wireframeDesc = {};
	wireframeDesc.FillMode = D3D11_FILL_WIREFRAME;
	wireframeDesc.CullMode = D3D11_CULL_NONE;
	wireframeDesc.FrontCounterClockwise = FALSE;

	resultFunc = device_->CreateRasterizerState(&rasterizerDesc, rasterizerState_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));
	resultFunc = device_->CreateRasterizerState(&wireframeDesc, wireframeRasterizerState_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));
	//deviceContext_->RSSetState(rasterizerState_);
	DisableWireframe();
}

void DX3DManager::InitBlend() {
	HRESULT resultFunc = S_OK;

	///
	/// ブレンド
	/// ＜構造体の詳細な説明＞ https://learn.microsoft.com/ja-jp/windows/win32/api/d3d11/ns-d3d11-d3d11_blend_desc
	/// 
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.RenderTarget[0].BlendEnable = TRUE; 
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	resultFunc = device_->CreateBlendState(&blendDesc, blendState_.GetAddressOf());
	assert(SUCCEEDED(resultFunc));

	float blendFactor[4] = { 0, 0, 0, 0 };
	deviceContext_->OMSetBlendState(blendState_.Get(), blendFactor, 0xFFFFFFFF);
}

void DX3DManager::Resize(float width, float height) {
	if (width == 0 || height == 0) {
		return;
	}

	deviceContext_->OMSetRenderTargets(0, nullptr, nullptr);

	if (renderTargetView_) { 
		renderTargetView_->Release();
		renderTargetView_ = nullptr; 
	}
	if (texture2D_) { 
		texture2D_->Release();
		texture2D_ = nullptr; 
	}
	if (depthStencilView_) {
		depthStencilView_->Release();
		depthStencilView_ = nullptr;
	}
	if (depthStencilTexture_) {
		depthStencilTexture_->Release();
		depthStencilTexture_ = nullptr;
	}

	deviceContext_->ClearState();
	deviceContext_->Flush();

	HRESULT hr = swapChain_->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	assert(SUCCEEDED(hr));

	hr = swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)texture2D_.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = device_->CreateRenderTargetView(texture2D_.Get(), nullptr, renderTargetView_.GetAddressOf());
	assert(SUCCEEDED(hr));

	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = width;
	depthDesc.Height = height;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 1;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	hr = device_->CreateTexture2D(&depthDesc, nullptr, depthStencilTexture_.GetAddressOf());
	assert(SUCCEEDED(hr));
	hr = device_->CreateDepthStencilView(depthStencilTexture_.Get(), nullptr, depthStencilView_.GetAddressOf());
	assert(SUCCEEDED(hr));

	deviceContext_->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), depthStencilView_.Get());

	D3D11_VIEWPORT viewport = {};
	viewport.Width = (float)width;
	viewport.Height = (float)height;
	viewport.MaxDepth = 1.0f;
	deviceContext_->RSSetViewports(1, &viewport);

	float blendFactor[4] = { 0, 0, 0, 0 };
	deviceContext_->OMSetBlendState(blendState_.Get(), blendFactor, 0xFFFFFFFF);
}

ID3D11Device* DX3DManager::GetDevice() { return device_.Get(); }
ID3D11DeviceContext* DX3DManager::GetDeviceContext() { return deviceContext_.Get(); }
IDXGISwapChain* DX3DManager::GetSwapChain() { return swapChain_.Get(); }
ID3D11RenderTargetView* DX3DManager::GetRTV() { return renderTargetView_.Get(); }
ID3D11Texture2D* DX3DManager::GetTexture2D() { return texture2D_.Get(); }
ID3D11Texture2D* DX3DManager::GetDepthTexture2D() { return depthStencilTexture_.Get(); }
ID3D11DepthStencilState* DX3DManager::GetDepthState() { return depthState_.Get(); }
ID3D11DepthStencilView* DX3DManager::GetDepthView() { return depthStencilView_.Get(); }
ID3D11RasterizerState* DX3DManager::GetRasterizerState() { return rasterizerState_.Get(); }
ID3D11BlendState* DX3DManager::GetBlendState() { return blendState_.Get(); }

void DX3DManager::EnableZDepthWrite() {
	GetDeviceContext()->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), GetDepthView());
}

void DX3DManager::DisableZDepthWrite() {
	GetDeviceContext()->OMSetRenderTargets(1, renderTargetView_.GetAddressOf(), nullptr);
}

void DX3DManager::EnableWireframe() {
	deviceContext_->RSSetState(wireframeRasterizerState_.Get());
}

void DX3DManager::DisableWireframe() {
	deviceContext_->RSSetState(rasterizerState_.Get());
}
