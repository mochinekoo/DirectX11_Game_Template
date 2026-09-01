Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wvpMatrix;           // ワールド・ビュー・プロジェクション行列
    float4 diffuse;             // 物体の色
    float4 ambient;             // 光が当たっていない部分の色（環境光）
    float4 specular;            // 光が当たった部分の色（鏡面反射）
    float3 emission;            // 物体が自ら発光する色
    float shininess;            // 輝きの強さ
    int hasTexture;             // テクスチャを持っているかどうか
    float3 lightDirection;      // ライトの向き
};

struct VSInput {
    float3 position : POSITION; // 頂点の位置
    float3 normal : NORMAL;     // 法線
    float4 color : COLOR;       // 色
    float2 uv : TEXCOORD;       // UV座標
};

struct VSOutput
{
    float4 position : SV_POSITION;  // 頂点の位置
    float3 normal : NORMAL;         // 法線
    float4 color : COLOR;           // 色
    float2 uv : TEXCOORD;           // UV座標
};


VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(float4(input.position, 1.0f), wvpMatrix);
    output.normal = input.normal;
    output.color = diffuse;
    output.uv = input.uv;

    return output;
}