#pragma once

#include "DXUtils.h"
#include "Bindable.h"

struct DirectionalLight {
	DirectX::XMFLOAT3 color, direction;
};

struct PointLight {
	DirectX::XMFLOAT3 color, position;

};

class Lighting {
public:

	void addLight(const DirectionalLight& light);
	void addLight(const PointLight& light);

private:
	std::vector<DirectionalLight> directionals;
	std::vector<PointLight> points;
};

