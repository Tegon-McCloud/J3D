
#include "VSCBuffers.hlsli"

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


Output main( Input input ) {
    
    Output output;
    
    output.viewPos = mul(float4(input.pos, 1.0f), modelView).xyz;
    output.normal = mul(input.normal, (float3x3) modelView);
    output.texCoord = input.texCoord;
    output.pos = mul(float4(output.viewPos, 1.0f), projection);
    
    return output;
}