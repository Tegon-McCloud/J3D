
#include "PSUtils.hlsli"
#include "CommonUtils.hlsli"

float4 main(PSInput input) : SV_TARGET {
    
    float3 color = 0.0f.xxx;
    
    float3 albedo = getColor(input.texCoords);
    
    uint numLights;
    uint lightStride;
    directionalLights.GetDimensions(numLights, lightStride);
       
    for (uint i = 0; i < numLights; i++) {
        float3 L = -directionalLights[i].direction;
        float3 N = getNormal(input.texCoords, input.tbn);
        
        color += albedo * max(0.0f, dot(L, N));
    }
    
    return float4(color, 1.0f);
    
}