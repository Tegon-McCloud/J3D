
float4 getColor(in float2 texcoords);

#ifdef COLOR_MAP
Texture2D colorMap;
SamplerState colorSampler;

float4 getColor(in float2 texcoords) {
    return colorMap.Sample(colorSampler, texcoords);
}
#else
float4 getColor(in float2 texcoords) {
    return 1.0f.rrrr;
}
#endif

float3 getNormal(in float2 texcoord, in float3x3 tbn);

#ifdef NORMAL_MAP
Texture2D normalMap;
SamplerState normalSampler;

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

#ifdef METAL_ROUGHNESS_MAP
Texture2D metalRoughnessMap;
#endif

