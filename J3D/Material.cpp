#include "Material.h"

Material::Material(Graphics& gfx, const MaterialConstants& constants) : constants(constants) {}

void Material::setNormalMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler) {

}

void Material::bind(Graphics& gfx) {
	for (size_t i = 0; i < textures.size(); i++) {
		textures[i]->bind(gfx, i);
		samplers[i]->bind(gfx, i);
	}
}
