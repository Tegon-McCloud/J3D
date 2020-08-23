#pragma once

class Graphics;

#include "DXUtils.h"
#include "Bindable.h"
#include "Texture.h"
#include "Sampler.h"

#include <type_traits>
#include <array>

struct MaterialConstants {
	float color[4];
	float metallic, roughness;
};

class Material : public Bindable {
public:
	using Reusable = std::false_type;
	Material(Graphics& gfx, const MaterialConstants& constants);

	void setColorMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler);
	void setNormalMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler);
	
	bool hasColorMap() const;
	bool hasNormalMap() const;

	void bind(Graphics& gfx) override;

private:

	MaterialConstants constants;
	
	std::shared_ptr<ShaderTexture2D> pColorTexture;
	std::shared_ptr<ShaderTexture2D> pMetallicRoughnessTexture;
	std::shared_ptr<ShaderTexture2D> pNormalTexture;
	std::shared_ptr<ShaderTexture2D> pAmbientOcclusionTexture;
	std::shared_ptr<ShaderTexture2D> pEmissiveTexture;
	
	std::shared_ptr<Sampler> pColorSampler;
	std::shared_ptr<Sampler> pMetallicRoughnessSampler;
	std::shared_ptr<Sampler> pNormalSampler;
	std::shared_ptr<Sampler> pAmbientOcclusionSampler;
	std::shared_ptr<Sampler> pEmissiveSampler;
};

