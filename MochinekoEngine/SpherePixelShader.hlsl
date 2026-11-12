Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix wvpMatrix; // ワールド・ビュー・プロジェクション行列
    matrix worldMatrix; // ワールド行列
    float4 colorList[2];
};

struct PSInput
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET {
    float4 color = input.color;
    return color;
}