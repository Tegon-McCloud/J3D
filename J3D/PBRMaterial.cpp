#include "PBRMaterial.h"

#include "Graphics.h"
#include "ConstantBuffer.h"

#include "PSUtilsPBR.hlsli"

#include <assimp/scene.h>
#include <assimp/pbrmaterial.h>

#include <filesystem>

PBR::Material::Material(Graphics& gfx, const std::filesystem::path& file, const aiMaterial& aiMat) {

	aiColor4D color(0.0f, 0.0f, 0.0f, 0.0f);
	if (aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, color) != AI_SUCCESS ||
		aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, constants.metallic) != AI_SUCCESS ||
		aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, constants.roughness) != AI_SUCCESS) {
		throw std::runtime_error("");
	}

	constants.color[0] = color.r;
	constants.color[1] = color.g;
	constants.color[2] = color.b;
	constants.color[3] = color.a;


	aiString colorTexFile;
	if (aiMat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &colorTexFile) == AI_SUCCESS) {
		std::filesystem::path fullPath = file.parent_path() / colorTexFile.C_Str();
		pColorTexture = gfx.getResourceMgr<PSTexture2D>().resolve(fullPath.generic_string(), fullPath);
		pColorSampler = gfx.getResourceMgr<PSSampler>().resolve("default", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
	}

	aiString normalTexFile;
	if (aiMat.GetTexture(aiTextureType_NORMALS, 0, &normalTexFile) == AI_SUCCESS) {
		std::filesystem::path fullPath = file.parent_path() / normalTexFile.C_Str();
		pNormalTexture = gfx.getResourceMgr<PSTexture2D>().resolve(fullPath.generic_string(), fullPath);
		pNormalSampler = gfx.getResourceMgr<PSSampler>().resolve("default", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP);
	}

}

void PBR::Material::bind(Graphics& gfx) {
	if (pColorTexture) {
		pColorTexture->bind(gfx, COLOR_MAP_SLOT);
		pColorSampler->bind(gfx, COLOR_SAMPLER_SLOT);
	}

	if (pMetallicRoughnessTexture) {
		pMetallicRoughnessTexture->bind(gfx, METALLIC_ROUGHNESS_MAP_SLOT);
		pMetallicRoughnessSampler->bind(gfx, METALLIC_ROUGHNESS_SAMPLER_SLOT);
	}

	if (pNormalTexture) {
		pNormalTexture->bind(gfx, NORMAL_MAP_SLOT);
		pNormalSampler->bind(gfx, NORMAL_SAMPLER_SLOT);
	}

	if (pAmbientOcclusionTexture) {
		pAmbientOcclusionTexture->bind(gfx, AMBIENT_OCCLUSION_MAP_SLOT);
		pAmbientOcclusionSampler->bind(gfx, AMBIENT_OCCLUSION_SAMPLER_SLOT);
	}

	if (pEmissiveSampler) {
		pEmissiveSampler->bind(gfx, EMISSIVE_MAP_SLOT);
		pEmissiveSampler->bind(gfx, EMISSIVE_SAMPLER_SLOT);
	}

	auto pMaterialCBuffer = gfx.getResourceMgr<PSConstantBuffer>().get("material");

	if (pMaterialCBuffer) {
		pMaterialCBuffer->set(gfx, constants);
	}
}

void PBR::Material::getDefines(std::vector<D3D_SHADER_MACRO>& macros) const {

	macros.push_back({ "PBR", nullptr });

	if (pColorTexture) {
		macros.push_back({ "COLOR_MAP", nullptr });
	}

	if (pMetallicRoughnessTexture) {
		macros.push_back({ "METALLIC_ROUGHNESS_MAP", nullptr });
	}

	if (pNormalTexture) {
		macros.push_back({ "NORMAL_MAP", nullptr });
	}
}

std::filesystem::path PBR::Material::getShaderFile() const {
	return "PSPBR.hlsl";
}

bool PBR::Material::isCompatible(const aiMaterial& aiMat) {

	aiColor4D color(0.0f, 0.0f, 0.0f, 0.0f);
	float metallic, roughness;
	if (aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, color) != AI_SUCCESS ||
		aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallic) != AI_SUCCESS ||
		aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughness) != AI_SUCCESS) {
		return false;
	}

	return true;

}
