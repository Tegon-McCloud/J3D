
#define DIRECTIONAL_LIGHT_SLOT 0
#define POINT_LIGHT_SLOT 1
#define SPOT_LIGHT_SLOT 2

#define COLOR_MAP_SLOT 3
#define METALLIC_ROUGHNESS_MAP_SLOT 4
#define NORMAL_MAP_SLOT 5
#define AMBIENT_OCCLUSION_MAP_SLOT 6
#define EMISSIVE_MAP_SLOT 7

#define COLOR_SAMPLER_SLOT 3
#define METALLIC_ROUGHNESS_SAMPLER_SLOT 4
#define NORMAL_SAMPLER_SLOT 5
#define AMBIENT_OCCLUSION_SAMPLER_SLOT 6
#define EMISSIVE_SAMPLER_SLOT 7

#ifndef __cplusplus

#define CONCAT(t, n) t##n

// Lights
struct DirectionalLight {
    float3 radiance;
    float3 direction;
};

StructuredBuffer<DirectionalLight> directionalLights : register(CONCAT(t, DIRECTIONAL_LIGHT_SLOT));

// Material cbuffer
struct Material {
    float4 baseColor;
    float metalness;
    float roughness;
};

cbuffer material : register(b0) {
    Material material;
    float2 pad;
};


// color
#ifdef COLOR_MAP
Texture2D colorMap : register(CONCAT(t, COLOR_MAP_SLOT));
SamplerState colorSampler : register(CONCAT(s, COLOR_SAMPLER_SLOT));
#endif

float4 getColor(in float2 texcoords) {
#ifdef COLOR_MAP
    return colorMap.Sample(colorSampler, texcoords) * material.baseColor;
#else
    return material.baseColor;
#endif
}

// metallic and roughness
#ifdef METALLIC_ROUGHNESS_MAP
Texture2D metalnessRoughnessMap : register(CONCAT(t, METALLIC_ROUGHNESS_MAP_SLOT));
SamplerState metalnessRougnessSampler : register(CONCAT(s, METALLIC_ROUGHNESS_SAMPLER_SLOT));
#endif

float2 getMetalnessRoughness(in float2 texcoord) {
#ifdef METALLIC_ROUGHNESS_MAP
    return metalnessRoughnessMap.Sample(metalnessRougnessSampler, texcoord).gb * float2(material.metalness, material.roughness);
#else
    return float2(material.metalness, material.roughness);
#endif
    
}

// normal

#ifdef NORMAL_MAP
Texture2D normalMap : register(CONCAT(t, NORMAL_MAP_SLOT));
SamplerState normalSampler : register(CONCAT(s, NORMAL_SAMPLER_SLOT));
#endif

float3 getNormal(in float2 texcoord, in float3x3 tbn) {
#ifdef NORMAL_MAP
    float3 normal = normalMap.Sample(normalSampler, texcoord).xyz;
    normal.y = 1.0f - normal.y;
    normal *= 2.0f;
    normal -= 1.0f.xxx;
    
    return normalize(mul(normal, tbn));
#else
    return normalize(mul(float3(0.0f, 0.0f, 1.0f), tbn));
#endif
}


#endif
