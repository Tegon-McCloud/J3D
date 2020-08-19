
#include "PSUtils.hlsli"
#include "CommonUtils.hlsli"

float4 main(PSInput input) : SV_TARGET {
    return getColor(input.texCoords);
}