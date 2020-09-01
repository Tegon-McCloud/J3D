
#include "CommonUtils.hlsli"
#include "PSUtilsPBR.hlsli"

static const float PI = 3.14159265359f;
static const float Epsilon = 0.001f;

static const float3 FDielectric = 0.04f;

float distributionGGX(float3 N, float3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0f);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0f) + 1.0f);
    denom = PI * denom * denom;

    return num / denom;
}

float geometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0f);
    float k = (r * r) / 8.0f;

    float num = NdotV;
    float denom = NdotV * (1.0f - k) + k;

    return num / denom;
}

float geometrySmith(float3 N, float3 V, float3 L, float roughness) {
    float NdotV = max(dot(N, V), 0.0f);
    float NdotL = max(dot(N, L), 0.0f);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

float3 fresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

float3 cookTorrance(
    in float3 albedo,
    in float metallic,
    in float roughness,
    in float3 irradiance,
    in float3 N,
    in float3 V,
    in float3 L) {
    
    float3 H = normalize(L + V);
    
    float3 F0 = lerp(FDielectric, albedo, metallic);
    float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);
    
    float numer = distributionGGX(N, H, roughness) * geometrySmith(N, V, L, roughness) * F;
    float denom = 4.0f * max(dot(N, V), 0.0f) * max(dot(N, L), 0.0f);
    
    float fcookTorrance = numer / max(denom, 0.001f);
    
    float3 ks = F;
    float3 kd = (1.0f - ks) * (1.0f - metallic);
    
    float fr = kd * albedo / PI + ks * fcookTorrance;
    
    return fr * max(dot(N, L), 0.0f) * irradiance;

}



float4 main(PSInput input) : SV_TARGET{

    float3 albedo = getColor(input.texCoords).rgb;
    float2 metallicRoughness = getMetallicRoughness(input.texCoords);
    float metallic = metallicRoughness.r;
    float roughness = metallicRoughness.g;

    float3 N = getNormal(input.texCoords, input.tbn);
    float3 V = normalize(cameraPosition - input.position);

    float3 Lo = 0.0f.rrr;

    uint numLights;
    uint lightStride;
    directionalLights.GetDimensions(numLights, lightStride);
       
    for (uint i = 0; i < numLights; i++) {
        float3 L = -directionalLights[i].direction;
        float3 radiance = directionalLights[i].radiance;
           
        Lo += cookTorrance(albedo, metallic, roughness, radiance, N, V, L);
        
    }
    
    pointLights.GetDimensions(numLights, lightStride);
    
    for (uint i = 0; i < numLights; i++) {
        float3 L = pointLights[i].position - input.position;
        float attenuation = 1.0f / dot(L, L);
        L = normalize(L);
        float3 radiance = attenuation * pointLights[i].radiance;
        
        Lo += cookTorrance(albedo, metallic, roughness, radiance, N, V, L);
    }
    
    float3 ambient = 0.3.rrr * albedo;
    
    return float4(Lo + ambient, 1.0f);
}