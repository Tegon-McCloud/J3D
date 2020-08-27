
#include "CommonUtils.hlsli"
#include "PSUtils.hlsli"

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

float4 main(PSInput input) : SV_TARGET{

    float3 albedo = getColor(input.texCoords).rgb;
    float2 metallicRoughness = getMetallicRoughness(input.texCoords);
    float metallic = metallicRoughness.r;
    float roughness = metallicRoughness.g;

    float3 N = getNormal(input.texCoords, input.tbn);
    float3 V = normalize(cameraPosition - input.position);

    float3 F0 = lerp(FDielectric, albedo, metallic);

    float3 Lo = 0.0f.rrr;

    uint numLights;
    uint lightStride;
    directionalLights.GetDimensions(numLights, lightStride);
       
    for (uint i = 0; i < numLights; i++) {
        float3 L = -directionalLights[i].direction;
        float3 H = normalize(V + L);

        float3 radiance = -directionalLights[i].radiance;

        float NDF = distributionGGX(N, H, roughness);
        float G = geometrySmith(N, V, L, roughness);
        float3 F = fresnelSchlick(max(dot(H, V), 0.0f), F0);

        float3 kS = F;
        //float3 kD = 1.0f.rrr - kS;
        //kD *= 1.0f - metallic;
        float3 kD = 1.0f.rrr;

        float3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        float3 specular = numerator / max(denominator, 0.001f);
        
        float NdotL = max(dot(N, L), 0.0f);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
    
    return float4(Lo, 1.0f);   
}