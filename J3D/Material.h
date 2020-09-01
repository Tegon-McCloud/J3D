#pragma once

class Graphics;
class aiMaterial;

#include "DXUtils.h"
#include "Bindable.h"
#include "Texture.h"
#include "Sampler.h"

#include <type_traits>
#include <vector>

class Material : public Bindable {
public:
	using Reusable = std::false_type;

	virtual void getDefines(std::vector<D3D_SHADER_MACRO>& macros) const = 0;
	virtual std::filesystem::path getShaderFile() const = 0;

};
