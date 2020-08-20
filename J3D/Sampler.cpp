#include "Sampler.h"

#include "Graphics.h"

Sampler::Sampler(Graphics& gfx, D3D11_TEXTURE_ADDRESS_MODE uMode, D3D11_TEXTURE_ADDRESS_MODE vMode, uint8_t slot) : slot(slot) {
	D3D11_SAMPLER_DESC desc;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;
	desc.AddressU = uMode;
	desc.AddressV = vMode;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_LESS;
	desc.BorderColor[0] = 0.0f;
	desc.BorderColor[1] = 0.0f;
	desc.BorderColor[2] = 0.0f;
	desc.BorderColor[3] = 0.0f;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;

	tif(gfx.getDevice().CreateSamplerState(&desc, &pSampler));
}

void VSSampler::bind(Graphics& gfx) {
	gfx.getContext().VSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

void VSSampler::bind(Graphics& gfx, uint8_t slot) {
	gfx.getContext().VSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

void PSSampler::bind(Graphics& gfx) {
	gfx.getContext().PSSetSamplers(slot, 1, pSampler.GetAddressOf());
}

void PSSampler::bind(Graphics& gfx, uint8_t slot) {
	gfx.getContext().PSSetSamplers(slot, 1, pSampler.GetAddressOf());
}


