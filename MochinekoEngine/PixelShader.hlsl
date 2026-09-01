Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wvpMatrix;       // ワールド・ビュー・プロジェクション行列
    float4 diffuse;         // 物体の色
    float4 ambient;         // 光が当たっていない部分の色（環境光）
    float4 specular;        // 光が当たった部分の色（鏡面反射）
    float3 emission;        // 物体が自ら発光する色
    float shininess;        // 輝きの強さ
    int hasTexture;         // テクスチャを持っているかどうか
    float3 lightDirection;  // ライトの向き
};

struct PSInput {
    float4 position : SV_POSITION;  // 頂点の位置
    float3 normal : NORMAL;         // 法線
    float4 color : COLOR;           // 色
    float2 uv : TEXCOORD0;          // UV座標
};

float4 main(PSInput input) : SV_TARGET {
    float4 color = input.color;
    
    if (hasTexture == 1) {
        color = texture0.Sample(sampler0, input.uv);
    }
    else {
        color = diffuse;
    }
    
    return color;
}