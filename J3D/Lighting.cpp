#include "Lighting.h"

void Lighting::addLight(const DirectionalLight& light) {
	directionals.emplace_back(light);
}

void Lighting::addLight(const PointLight& light) {
	points.push_back(light);
}
