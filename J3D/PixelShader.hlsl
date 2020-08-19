
#include "PSUtils.hlsli"
#include "CommonUtils.hlsli"

Texture2D tex;
SamplerState samplerState;

float4 main(PSInput input) : SV_TARGET {
    
    float3 color = tex.Sample(samplerState, input.texCoord).rgb;
    color.g = 1.0f - color.g;
    
	return float4(color, 1.0f);
}