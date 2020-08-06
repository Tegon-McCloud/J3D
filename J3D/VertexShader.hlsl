
struct Input {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

struct Output
{
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 pos : SV_POSITION;
};

cbuffer mvp : register(b0) {
    matrix transform;
};

Output main( Input input ) {
    
    Output output;
    
    output.viewPos = 0.0f.xxx;
    output.normal = 0.0f.xxx;
    output.texcoord = 0.0f.xx;
    output.pos = mul(transform, float4(input.pos, 1.0f));
    
    return output;
}