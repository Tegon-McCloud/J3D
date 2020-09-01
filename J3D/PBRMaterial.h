#pragma once

#include "Material.h"

namespace PBR {
	struct MaterialConstants {
		float color[4];
		float metallic, roughness;
	};

	class Material : public ::Material {
	public:
		Material(Graphics& gfx, const std::filesystem::path& file, const aiMaterial& aiMat);

		void bind(Graphics& gfx) override;

		void getDefines(std::vector<D3D_SHADER_MACRO>& macros) const override;

		static bool isCompatible(const aiMaterial& aiMat);

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
}
