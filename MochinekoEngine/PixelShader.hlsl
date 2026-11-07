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
    int colorInversion;
    int colorBinary;
    
    // セピア
    int colorSepia;
    float sepiaStrength;
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
        color = texture0.Sample(sampler0, input.uv) * color;
    }
    else {
        color = diffuse;
    }
    
    if (colorInversion == 1) {
        color.r = 1.0f - color.r;
        color.g = 1.0f - color.g;
        color.b = 1.0f - color.b;
    }
    
    if (colorSepia == 1) {
        float3 original = color.rgb;

        float3 sepia;
        sepia.r = dot(original, float3(0.393, 0.769, 0.189));
        sepia.g = dot(original, float3(0.349, 0.686, 0.168));
        sepia.b = dot(original, float3(0.272, 0.534, 0.131));

        sepia = saturate(sepia);

        color.rgb = lerp(original, sepia, saturate(sepiaStrength));
    }
    
    float3 normalizelLight = normalize(-lightDirection);    // ライトの正規化
    float3 normalizeNormal = normalize(input.normal);   // 法線の正規化    
    float diffuseFactor = max(dot(normalizeNormal, normalizelLight), 0.0f); // 法線とライトの向きの内積で、面に当たる光の強さを求める。光が当たらない面は0になる。
    float3 minAmbient = float3(0.15f, 0.15f, 0.15f);    // 最小の明るさ
    float3 baseAmbient = max(ambient.rgb, minAmbient);  // ライトが暗くならないようにするために、maxで比較する
    float3 lighting = saturate(baseAmbient + diffuseFactor);    // 明るさは0～1なので、saturateで
    color.rgb *= lighting;  // ライティングした後の色を求める
    
    if (enableGray == 1) {
        float r = color.r * 0.299;
        float g = color.g * 0.587;
        float b = color.b * 0.114;
        float gray = r + g + b;
        return float4(gray, gray, gray, color.w);
    }
    
    return color;
}