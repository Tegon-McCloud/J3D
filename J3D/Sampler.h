#pragma once

class Graphics;

#include "DXUtils.h"
#include "Bindable.h"

class Sampler : public Bindable {
public:
	Sampler(Graphics& gfx, D3D11_TEXTURE_ADDRESS_MODE uMode, D3D11_TEXTURE_ADDRESS_MODE vMode, uint8_t slot = 0);

	virtual void bind(Graphics& gfx, uint8_t slot) = 0;

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	uint8_t slot;

};

class VSSampler : public Sampler {
public:
	using Sampler::Sampler;

	void bind(Graphics& gfx) override;
	void bind(Graphics& gfx, uint8_t slot) override;
};

class PSSampler : public Sampler {
public:
	using Sampler::Sampler;

	void bind(Graphics& gfx) override;
	void bind(Graphics& gfx, uint8_t slot) override;
};
