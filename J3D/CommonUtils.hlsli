
typedef struct {
    float3 pos : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float2 texCoords : TEXCOORD;
} VSInput;

typedef struct {
    float3 position : POSITION;
    float2 texCoords : TEXCOORD;
    float3x3 tbn : TBN;
    float4 screenPos : SV_POSITION;
} VSOutput, PSInput;

