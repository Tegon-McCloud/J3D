
#include "CommonUtils.hlsli"

Texture2D tex;
SamplerState samplerState;

float4 main(PSInput input) : SV_TARGET {
    
    //input.texCoord.g = 1.0f - input.texCoord.g;
    float3 color = tex.Sample(samplerState, input.texCoord).rgb;
    //normal.g = 1.0f - normal.g;
    
	return float4(color, 1.0f);
}