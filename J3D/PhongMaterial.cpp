#include "PhongMaterial.h"

#include <assimp/scene.h>
#include <assimp/material.h>

Phong::Material::Material(Graphics& gfx, std::filesystem::path, const aiMaterial& aiMat) {

}

bool Phong::Material::isCompatible(const aiMaterial& aiMat) {
	aiColor4D color = { 0.0f, 0.0f, 0.0f, 0.0f };
	if (aiMat.Get(AI_MATKEY_COLOR_DIFFUSE, color) != AI_SUCCESS) {

	}

	return true;
}
