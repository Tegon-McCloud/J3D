
Texture2D tex;
SamplerState samplerState;

struct Input {
    float3 viewPos : POSITION;
    float3 viewNormal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 pos : SV_POSITION;
};

float4 main(Input input) : SV_TARGET {
	return float4(tex.Sample(samplerState, input.texcoord).rgb, 1.0f);
}