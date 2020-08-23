#include "Lighting.h"

void Lighting::addLight(const DirectionalLight& light) {
	directionals.emplace_back(light);
}
