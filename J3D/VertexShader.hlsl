
#include "CommonUtils.hlsli"
#include "VSCBuffers.hlsli"

VSOutput main( VSInput input ) {
    
    VSOutput output;
    
    output.viewPos = mul(float4(input.pos, 1.0f), modelView).xyz;
    float3 normal = mul((float3) input.normal, (float3x3) modelView);
    float3 tangent = mul((float3) input.tangent, (float3x3) modelView);
    float3 bitangent = cross(normal, tangent);
    output.tbn = float3x3(normal, tangent, bitangent);

    output.texCoords = input.texCoords;
    output.pos = mul(float4(output.viewPos, 1.0f), projection);
    
    return output;
}