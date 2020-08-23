
typedef struct {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORD;
} VSInput;

typedef struct {
    float3 viewPos : POSITION;
    float2 texCoords : TEXCOORD;
    float3x3 tbn : TBN;
    float4 pos : SV_POSITION;
} VSOutput, PSInput;
