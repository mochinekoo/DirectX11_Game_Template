#include "ShaderManager.h"
#include <map>
#include <d3dcompiler.h>
#include "DX3D.h"
using namespace DX3D;

namespace ShaderManager {
    std::map<GamePixelShaderType, GamePixelShader> pixelShaderList;
    std::map<GameVertexShaderType, GameVertexShader> vertexShaderList;
}

int ShaderManager::initialize() {
    HRESULT result = S_OK;
    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;

    D3DCompileFromFile(
        L"TestPS.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &psBlob,
        nullptr
    );


    D3DCompileFromFile(
        L"TestVS.hlsl",
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "main",
        "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS,
        0,
        &vsBlob,
        nullptr
    );

    GameVertexShader testVertexShader = {};
    DX3D::GetDevice()->CreateVertexShader(
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        nullptr,
        &testVertexShader.vertexShader_
    );

    D3D11_INPUT_ELEMENT_DESC inputElementDesc[] = {
    { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
    };

    result = GetDevice()->CreateInputLayout(
        inputElementDesc,
        ARRAYSIZE(inputElementDesc),
        vsBlob->GetBufferPointer(),
        vsBlob->GetBufferSize(),
        &testVertexShader.inputLayout_
    );


    vertexShaderList[TEST_VERTEX_SHADER] = testVertexShader;

    GamePixelShader testPixelShader = {};
    DX3D::GetDevice()->CreatePixelShader(
        psBlob->GetBufferPointer(),
        psBlob->GetBufferSize(),
        nullptr,
        &testPixelShader.pixelShader_
    );
    pixelShaderList[TEST_PIXEL_SHADER] = testPixelShader;

    return 0;
}

GamePixelShader ShaderManager::GetPixelShader(GamePixelShaderType type) {
    return pixelShaderList[type];
}

GameVertexShader ShaderManager::GetVertexShader(GameVertexShaderType type) {
    return vertexShaderList[type];
}
