
struct Input {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
};

struct Output
{
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float4 pos : SV_POSITION;
};

cbuffer modelView : register(b0) {
    float4x4 modelView;
};

cbuffer projection : register(b1) {
    float4x4 projection;
}

Output main( Input input ) {
    
    Output output;
    
    output.viewPos = mul(modelView, float4(input.pos, 1.0f));
    output.normal = mul(input.normal, (float3x3)modelView);
    output.texCoord = input.texCoord;
    output.pos = mul(projection, float4(output.viewPos, 1.0f));
    
    return output;
}