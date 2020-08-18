
typedef struct {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float4 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
} VSInput;

typedef struct {
    float3 viewPos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoord : TEXCOORD;
    float4 pos : SV_POSITION;
} VSOutput, PSInput;
