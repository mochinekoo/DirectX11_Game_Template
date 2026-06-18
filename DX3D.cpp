#include "DX3D.h"
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>
#include <d3dcompiler.h>
#include "framework.h"

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DX3D {

	ID3D11Device* device_ = nullptr;
	ID3D11DeviceContext* context_ = nullptr;
	IDXGISwapChain* swapChain_ = nullptr;
	ID3D11RenderTargetView* renderTargetView_ = nullptr;
	ID3D11Texture2D* texture2D_ = nullptr;

	
	int initializeDX3D() {
		DXGI_SWAP_CHAIN_DESC desc = {};
		desc.BufferDesc.Width = GameWindow::WINDOW_WIDTH; //画面の幅
		desc.BufferDesc.Height = GameWindow::WINDOW_HEIGHT; //画面の幅
		desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; //各8ビットずつの色
		desc.Windowed = TRUE; //ウインドウモード
		desc.OutputWindow = GameWindow::mainHWND; //出力するウインドウ
		desc.BufferCount = 1; //バファー（裏画面）の数
		desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		desc.SampleDesc.Count = 1; //サンプル数
		desc.SampleDesc.Quality = 0; //品質レベル
		desc.BufferDesc.RefreshRate.Numerator = 60;
		desc.BufferDesc.RefreshRate.Denominator = 1;

		D3D_FEATURE_LEVEL level = {};
		UINT creationFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
		D3D11CreateDeviceAndSwapChain(
			nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			nullptr,
			0,
			D3D11_SDK_VERSION,
			&desc,
			&swapChain_,
			&device_,
			&level,
			&context_
		);

		swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&texture2D_);

		IDXGISurface* surface = nullptr;
		swapChain_->GetBuffer(0, IID_PPV_ARGS(&surface));

		device_->CreateRenderTargetView(texture2D_, NULL, &renderTargetView_);

		D3D11_VIEWPORT viewport = {};
		viewport.Width = GameWindow::WINDOW_WIDTH;
		viewport.Height = GameWindow::WINDOW_HEIGHT;
		viewport.MinDepth = 0.0f;
		viewport.MaxDepth = 1.0f;
		viewport.TopLeftX = 0;
		viewport.TopLeftY = 0;
		context_->RSSetViewports(1, &viewport);

		return 0;
	}

	ID3D11Device* GetDevice() {
		return device_;
	}

	ID3D11DeviceContext* GetDeviceContext() {
		return context_;
	}

	IDXGISwapChain* GetSwapChain() {
		return swapChain_;
	}

	ID3D11RenderTargetView* GetRenderTargetView() {
		return renderTargetView_;
	}

	ID3D11Texture2D* GetTexture2D() {
		return texture2D_;
	}

}