
float4 getColor();
float3 getNormal(float3 vertexNormal, float3 tangent);

#ifdef BASE_COLOR_MAP
Texture2D colorMap;
#endif

#ifdef NORMAL_MAP
Texture2D normalMap;
#endif

#ifdef METAL_ROUGHNESS_MAP
Texture2D metalRoughnessMap;
#endif

