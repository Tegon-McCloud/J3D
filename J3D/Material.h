#pragma once

#include "DXUtils.h"
#include "Bindable.h"
#include "Texture.h"

#include <type_traits>
#include <vector>

class Material : public Bindable {
public:
	using Reusable = std::false_type;



private:
	bool baseColorMap, normalMap, metalRoughnessMap;

	std::vector<std::shared_ptr<Texture2D>> textures;
	
};

