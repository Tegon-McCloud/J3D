#pragma once

#include "DXUtils.h"
#include "Bindable.h"

struct DirectionalLight {
	DirectX::XMFLOAT3 color, direction;
};

class Lighting {
public:

	void addLight(const DirectionalLight& light);
	


private:
	std::vector<DirectionalLight> directionals;
};

