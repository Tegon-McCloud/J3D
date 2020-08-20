// Material cbuffer
struct Material {
    float4 baseColor;
    float metallic;
    float roughness;
};

cbuffer material : register(b0) {
    Material material;
};

// texture declarations
float4 getColor(in float2 texcoords);

#ifdef COLOR_MAP
Texture2D colorMap : register(t0);
SamplerState colorSampler : register(s0);

float4 getColor(in float2 texcoords) {
    return colorMap.Sample(colorSampler, texcoords);
}
#else
float4 getColor(in float2 texcoords) {
    return material.baseColor;
}
#endif

float3 getNormal(in float2 texcoord, in float3x3 tbn);

#ifdef NORMAL_MAP
Texture2D normalMap : register(t1);
SamplerState normalSampler : register(s1);

float3 getNormal(in float2 texcoord, in float3x3 tbn) {
    float3 normal = normalMap.Sample(normalSampler, texcoord).xyz;
    normal *= 2.0f;
    normal -= 1.0f.xxx;
    return mul(normal, tbn);
}
#else
float3 getNormal(in float2 texcoord, in float3x3 tbn) {
    return float3(tbn._m02, tbn._m12, tbn._m22);
}
#endif


