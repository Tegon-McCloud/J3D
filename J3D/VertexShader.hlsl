
#include "CommonUtils.hlsli"
#include "VSUtils.hlsli"

VSOutput main( VSInput input ) {
    
    VSOutput output;
    
    output.viewPos = mul(float4(input.pos, 1.0f), modelView).xyz;
    float3 normal = mul((float3) input.normal, (float3x3) modelView);
    float3 tangent = mul((float3) input.tangent, (float3x3) modelView);
    float3 bitangent = cross(normal, tangent);
    output.tbn = float3x3(tangent, bitangent, normal);

    output.texCoords.r = input.texCoords.r;
    output.texCoords.g = -input.texCoords.g;
    
    output.pos = mul(float4(output.viewPos, 1.0f), projection);
    
    return output;
}