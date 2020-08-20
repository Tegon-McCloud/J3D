#include "Material.h"

Material::Material(const MaterialConstants& constants) : constants(constants) {}

void Material::setColorMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler) {
	
}

void Material::setNormalMap(std::shared_ptr<ShaderTexture2D> pTexture, std::shared_ptr<Sampler> pSampler) {

}

void Material::bind(Graphics& gfx) {
	for (size_t i = 0; i < textures.size(); i++) {
		textures[i]->bind(gfx, i);
		samplers[i]->bind(gfx, i);
	}
}

void Material::updateSlots() {
	
	uint8_t slot;
	for (size_t i = 0; i < textures.size(); i++) {
		if (textures[i]) {
			assert(samplers[i]);

			textures[i]->setSlot(slot);
			samplers[i]->setSlot(slot);
			slot++;
		}
	}

}