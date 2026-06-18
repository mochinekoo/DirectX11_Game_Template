#pragma once
#include <Windows.h>
#include <d3d11.h>
#include <d2d1.h>
#include <dwrite.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

namespace DX3D {

	int initializeDX3D();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	IDXGISwapChain* GetSwapChain();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11Texture2D* GetTexture2D();
}