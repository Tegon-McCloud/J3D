cbuffer model : register(b0) {
    float4x4 model;
};

cbuffer viewProjection : register(b1) {
    float4x4 viewProjection;
}
