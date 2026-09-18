Texture2D texture0 : register(t0);
SamplerState sampler0 : register(s0);

cbuffer ConstantBuffer : register(b0) {
    matrix wvpMatrix;
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
    
    if (enableGray == 1) {
        float r = color.r * 0.299;
        float g = color.g * 0.587;
        float b = color.b * 0.114;
        float gray = r + g + b;
        return float4(gray, gray, gray, color.w);
    }
    
    return color;
}