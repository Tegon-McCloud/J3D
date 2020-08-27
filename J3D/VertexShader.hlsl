
#include "CommonUtils.hlsli"
#include "VSUtils.hlsli"

VSOutput main( VSInput input ) {
    
    VSOutput output;
    
    output.position = mul(float4(input.pos, 1.0f), model).xyz;
    float3 normal = mul((float3) input.normal, (float3x3) model);
    float3 tangent = mul((float3) input.tangent, (float3x3) model);
    float3 bitangent = cross(normal, tangent);
    output.tbn = float3x3(tangent, bitangent, normal);

    output.texCoords.r = input.texCoords.r;
    output.texCoords.g = -input.texCoords.g;
    
    output.screenPos = mul(float4(output.position, 1.0f), viewProjection);
    
    return output;
}