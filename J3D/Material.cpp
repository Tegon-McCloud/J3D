#include "Material.h"

#include "Graphics.h"
#include "ConstantBuffer.h"

#include "PSUtils.hlsli"

Material::Material(Graphics& gfx, const MaterialConstants& constants) : constants(constants) {}

void Material::setColorMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler) {
	pColorTexture = std::move(pTexture);
	pColorSampler = std::move(pSampler);
}

void Material::setNormalMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler) {
	pNormalTexture = std::move(pTexture);
	pNormalSampler = std::move(pSampler);
}

bool Material::hasColorMap() const {
	return pColorTexture.operator bool();
}

bool Material::hasNormalMap() const {
	return pNormalTexture.operator bool();
}

void Material::bind(Graphics& gfx) {
	if (pColorTexture) {
		pColorTexture->bind(gfx, COLOR_MAP_SLOT);
		pColorSampler->bind(gfx, COLOR_SAMPLER_SLOT);
	}

	if (pMetallicRoughnessTexture) {
		pMetallicRoughnessTexture->bind(gfx, METALLIC_ROUGHNESS_MAP_SLOT);
		pMetallicRoughnessSampler->bind(gfx, METALLIC_ROUGHNESS_SAMPLER_SLOT);
	}

	if (pNormalTexture) {
		pNormalTexture->bind(gfx, NORMAL_MAP_SLOT);
		pNormalSampler->bind(gfx, NORMAL_SAMPLER_SLOT);
	}

	if (pAmbientOcclusionTexture) {
		pAmbientOcclusionTexture->bind(gfx, AMBIENT_OCCLUSION_MAP_SLOT);
		pAmbientOcclusionSampler->bind(gfx, AMBIENT_OCCLUSION_SAMPLER_SLOT);
	}

	if (pEmissiveSampler) {
		pEmissiveSampler->bind(gfx, EMISSIVE_MAP_SLOT);
		pEmissiveSampler->bind(gfx, EMISSIVE_SAMPLER_SLOT);
	}

	gfx.getResourceMgr<PSConstantBuffer>().get("material")->set(gfx, constants);

}
