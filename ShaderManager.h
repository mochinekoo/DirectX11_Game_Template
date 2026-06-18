#pragma once
#include <d3d11.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

struct GamePixelShader {
	ID3D11PixelShader* pixelShader_ = nullptr;
	ID3D11InputLayout* inputLayout_ = nullptr;
};

struct GameVertexShader {
	ID3D11VertexShader* vertexShader_ = nullptr;
	ID3D11InputLayout* inputLayout_ = nullptr;
};

enum GamePixelShaderType {
	TEST_PIXEL_SHADER,
};

enum GameVertexShaderType {
	TEST_VERTEX_SHADER
};

namespace ShaderManager {

	int initialize();
	GamePixelShader GetPixelShader(GamePixelShaderType type);
	GameVertexShader GetVertexShader(GameVertexShaderType type);

}