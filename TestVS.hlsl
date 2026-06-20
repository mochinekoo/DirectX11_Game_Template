cbuffer ConstantBuffer : register(b0) {
    matrix world;
    matrix view;
    matrix projection;
};

struct VS_INPUT {
    float3 pos : POSITION;
};

struct VS_OUTPUT {
    float4 pos : SV_POSITION;
};

VS_OUTPUT main(VS_INPUT input) {
    VS_OUTPUT output;
    float4 pos = float4(input.pos, 1.0f);
    float4 worldPos = mul(pos, world);
    float4 viewPos = mul(worldPos, view);
    output.pos = mul(viewPos, projection);
    return output;
}