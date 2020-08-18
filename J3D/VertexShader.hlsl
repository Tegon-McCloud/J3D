
#include "CommonUtils.hlsli"
#include "VSCBuffers.hlsli"

VSOutput main( VSInput input ) {
    
    VSOutput output;
    
    output.viewPos = mul(float4(input.pos, 1.0f), modelView).xyz;
    output.normal = mul(input.normal, (float3x3) modelView);
    output.texCoord = input.texCoord;
    output.pos = mul(float4(output.viewPos, 1.0f), projection);
    
    return output;
}