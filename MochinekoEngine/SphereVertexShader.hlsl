Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0)
{
    matrix wvpMatrix; // ワールド・ビュー・プロジェクション行列
    matrix worldMatrix; // ワールド行列
    float4 colorList[2];
};

struct VSInput
{
    float3 position : POSITION; // 頂点の位置
    float3 normal : NORMAL; // 法線
    float4 color : COLOR; // 色
    float2 uv : TEXCOORD; // UV座標
};

struct VSOutput
{
    float4 position : SV_POSITION; // 頂点の位置
    float3 normal : NORMAL; // 法線
    float4 color : COLOR; // 色
    float2 uv : TEXCOORD; // UV座標
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), wvpMatrix);
    float3x3 normalMatrix = (float3x3) worldMatrix;
    output.normal = normalize(mul(input.normal, normalMatrix));
    output.color = lerp(colorList[0], colorList[1], input.uv.y);
    output.uv = input.uv;
    
    return output;
}