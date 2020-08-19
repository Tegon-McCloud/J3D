#pragma once

#include "DXUtils.h"
#include "Bindable.h"

class Sampler : public Bindable {
public:
	Sampler(class Graphics& gfx, uint8_t slot);

protected:
	Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampler;
	uint8_t slot;

};

class VSSampler : public Sampler {
public:
	using Sampler::Sampler;

	void bind(class Graphics& gfx) override;
};

class PSSampler : public Sampler {
public:
	using Sampler::Sampler;

	void bind(class Graphics& gfx) override;
};
