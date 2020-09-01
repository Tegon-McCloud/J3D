#pragma once

#include "Material.h"

namespace Phong {

	struct MaterialConstants {
		float color[4];
		

	};

	class Material : public ::Material {
	public:
		Material(Graphics& gfx, std::filesystem::path, const aiMaterial& aiMat);

		std::filesystem::path getShaderFile() const override;

		static bool isCompatible(const aiMaterial& aiMat);

	};
}

