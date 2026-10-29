Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wvpMatrix;
    matrix worldMatrix;
    float4 diffuse;
    float4 ambient;
    float4 specular;
    float3 emission;
    float shininess;
    int hasTexture;
    float3 lightDirection;
    int enableGray;
};

struct PSInput {
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET {
    float4 color = input.color;
    
    if (hasTexture == 1) {
        color = texture0.Sample(sampler0, input.uv);
    }
    else {
        color = diffuse;
    }
    
    float3 normalizelLight = normalize(-lightDirection);    // ライトの正規化
    float3 normalizeNormal = normalize(input.normal);   // 法線の正規化    
    float diffuseFactor = max(dot(normalizeNormal, normalizelLight), 0.0f); // 法線とライトの向きの内積で、面に当たる光の強さを求める。光が当たらない面は0になる。
    float3 minAmbient = float3(0.15f, 0.15f, 0.15f);    // 最小の明るさ
    float3 baseAmbient = max(ambient.rgb, minAmbient);  // ライトが暗くならないようにするために、maxで比較する
    float3 lighting = saturate(baseAmbient + diffuseFactor);    // 明るさは0～1なので、saturateで
    color.rgb *= lighting;  // ライティングした後の色を求める
    color.a = 1.0f;
    
    if (enableGray == 1) {
        float r = color.r * 0.299;
        float g = color.g * 0.587;
        float b = color.b * 0.114;
        float gray = r + g + b;
        return float4(gray, gray, gray, color.w);
    }
    
    return color;
}