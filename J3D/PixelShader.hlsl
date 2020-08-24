
#include "PSUtils.hlsli"
#include "CommonUtils.hlsli"

static const float PI = 3.14159265359f;
static const float Epsilon = 0.001f;

static const float3 fDielectric = 0.04f;


float4 main(PSInput input) : SV_TARGET {
    
    float3 pxColor = 0.0f.xxx;
    
    float3 N = getNormal(input.texCoords, input.tbn);
    
    float3 albedo = getColor(input.texCoords);
    
    uint numLights;
    uint lightStride;
    directionalLights.GetDimensions(numLights, lightStride);
       
    for (uint i = 0; i < numLights; i++) {
        float3 L = -directionalLights[i].direction;
        
        pxColor += albedo  * max(0.0f, dot(L, N));
    }
    
    return float4(pxColor, 1.0f);
    
}