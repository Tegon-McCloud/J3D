#pragma once

class Graphics;

#include "DXUtils.h"
#include "Bindable.h"
#include "Texture.h"
#include "Sampler.h"

#include <type_traits>
#include <array>

struct MaterialConstants {
	float r, g, b, a;
	float metallic, roughness;
};

class Material : public Bindable {
public:
	using Reusable = std::false_type;
	Material(const MaterialConstants& constants);

	void setColorMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler);
	void setNormalMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler);
	
	void bind(Graphics& gfx) override;

private:

	MaterialConstants constants;
	bool hasColorMap, hasNormalMap;

	std::array<std::shared_ptr<ShaderTexture2D>, 2> textures;
	std::array<std::shared_ptr<Sampler>, 2> samplers;
	
};

