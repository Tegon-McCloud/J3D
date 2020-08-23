#include "Scene.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/pbrmaterial.h>

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstddef>
#include <sstream>
#include <functional>
#include <iostream>

using namespace DirectX;

SceneNode::SceneNode() : pParent(nullptr), parentToThis() {
	DirectX::XMStoreFloat4x4(&parentToThis, XMMatrixIdentity());
}

void SceneNode::draw(Graphics& gfx, DirectX::FXMMATRIX parentTransform) {
	
	XMMATRIX accumulated = XMMatrixMultiply(parentTransform, DirectX::XMLoadFloat4x4(&parentToThis));

	for (auto pChild : children) {
		pChild->draw(gfx, accumulated);
	}

	for(auto pMesh : meshes) {
		pMesh->draw(gfx, accumulated);
	}
}

void SceneNode::move(SceneNode* pNewParent) {
	if (pParent) {
		auto it = std::find(pParent->children.begin(), pParent->children.end(), this);
		if (it != pParent->children.end()) {
			pParent->children.erase(it);
		}
	}

	pNewParent->children.push_back(this);
	pParent = pNewParent;
}

void SceneNode::clear() {
	std::for_each(children.begin(), children.end(), [](SceneNode* pChild) -> void { pChild->pParent = nullptr; });
	children.clear();
}

void SceneNode::addMesh(Mesh* pMesh) {
	meshes.push_back(pMesh);
}

void SceneNode::transform(DirectX::FXMMATRIX transform) {
	DirectX::XMStoreFloat4x4(
		&parentToThis,
		XMMatrixMultiply(
			DirectX::XMLoadFloat4x4(&parentToThis),
			transform
		)
	);

}

Scene::Scene(Graphics& gfx, const std::filesystem::path& file) {
	
	Assimp::Importer imp;
	imp.ReadFile(file.generic_string(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
	aiScene* pAiScene = imp.GetOrphanedScene();

	std::vector<std::shared_ptr<Material>> materials;

	for (size_t i = 0; i < pAiScene->mNumMaterials; i++) {
		aiMaterial& aiMat = *pAiScene->mMaterials[i];

		MaterialConstants matConsts;
		
		aiColor4D color(0.0f, 0.0f, 0.0f, 0.0f);
		if (aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, color) != AI_SUCCESS ||
			aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, matConsts.metallic) != AI_SUCCESS ||
			aiMat.Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, matConsts.roughness) != AI_SUCCESS) {
			throw std::runtime_error("");
		}

		matConsts.color[0] = color.r;
		matConsts.color[1] = color.g;
		matConsts.color[2] = color.b;
		matConsts.color[3] = color.a;

		std::shared_ptr<Material> pMaterial = gfx.getResourceMgr<Material>().resolve("", matConsts);
		
		aiString colorTexFile;
		if (aiMat.GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_TEXTURE, &colorTexFile) == AI_SUCCESS) {
			std::filesystem::path fullPath = file.parent_path() / colorTexFile.C_Str();
			pMaterial->setColorMap(gfx.getResourceMgr<PSTexture2D>().resolve(fullPath.generic_string(), fullPath),
				gfx.getResourceMgr<PSSampler>().resolve("default", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP));
		}

		aiString normalTexFile;
		if (aiMat.GetTexture(aiTextureType_NORMALS, 0, &normalTexFile) == AI_SUCCESS) {
			std::filesystem::path fullPath = file.parent_path() / normalTexFile.C_Str();
			pMaterial->setNormalMap(gfx.getResourceMgr<PSTexture2D>().resolve(fullPath.generic_string(), fullPath), 
				gfx.getResourceMgr<PSSampler>().resolve("default", D3D11_TEXTURE_ADDRESS_WRAP, D3D11_TEXTURE_ADDRESS_WRAP));
		}

		materials.push_back(pMaterial);

		

	}


	for (size_t i = 0; i < pAiScene->mNumMeshes; i++) {
		meshes.emplace_back();
		Mesh& mesh = meshes.back();
		aiMesh* pAiMesh = pAiScene->mMeshes[i];

		DXUtils::VertexAttributes vertexAttribs;
		vertexAttribs.positionFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.normalFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.tangentFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.texcoordFormats.push_back(DXUtils::Format(DXUtils::AggregateType::VEC2, DXUtils::ComponentType::FLOAT));
		
		size_t vertexSize = vertexAttribs.getVertexSize();
		
		std::vector<std::byte> vertexData(pAiMesh->mNumVertices * vertexSize);

		for (size_t i = 0; i < pAiMesh->mNumVertices; i++) {
			size_t offset = vertexSize * i;
			float* pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.positionOffset()]);
			pData[0] = pAiMesh->mVertices[i].x;
			pData[1] = pAiMesh->mVertices[i].y;
			pData[2] = pAiMesh->mVertices[i].z;

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.normalOffset()]);
			pData[0] = pAiMesh->mNormals[i].x;
			pData[1] = pAiMesh->mNormals[i].y;
			pData[2] = pAiMesh->mNormals[i].z;

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.tangentOffset()]);
			pData[0] = pAiMesh->mTangents[i].x;
			pData[1] = pAiMesh->mTangents[i].y;
			pData[2] = pAiMesh->mTangents[i].z;

			assert(pAiMesh->GetNumUVChannels() > 0);
			assert(pAiMesh->mNumUVComponents[0] == 2);

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.texcoordOffset(0)]);
			pData[0] = pAiMesh->mTextureCoords[0][i].x;
			pData[1] = pAiMesh->mTextureCoords[0][i].y;
		}
		
		std::shared_ptr<VertexBuffer> pVB = gfx.getResourceMgr<VertexBuffer>().resolve("", vertexData, vertexAttribs);
		mesh.addBindable(std::move(pVB));
		
		std::vector<uint32_t> indexData(static_cast<size_t>(pAiMesh->mNumFaces) * 3);

		for (size_t i = 0; i < pAiMesh->mNumFaces; i++) {
			assert(pAiMesh->mFaces[i].mNumIndices == 3);

			indexData.push_back(pAiMesh->mFaces[i].mIndices[0]);
			indexData.push_back(pAiMesh->mFaces[i].mIndices[1]);
			indexData.push_back(pAiMesh->mFaces[i].mIndices[2]);
		}
		
		std::shared_ptr<IndexBuffer> pIB = gfx.getResourceMgr<IndexBuffer>().resolve("", indexData);
		mesh.addBindable(pIB);

		std::shared_ptr<Material> pMat = materials[pAiMesh->mMaterialIndex];
		mesh.addBindable(pMat);

		std::shared_ptr<VertexShader> pVS = gfx.getResourceMgr<VertexShader>().resolve(
			"vs", std::filesystem::path("./VertexShader.hlsl"));
		mesh.addBindable(pVS);

		std::shared_ptr<PixelShader> pPS = gfx.getResourceMgr<PixelShader>().resolve(
			"ps", std::filesystem::path("./PixelShader.hlsl"), pMat->hasColorMap(), pMat->hasNormalMap());
		mesh.addBindable(pPS);
		
	}

	std::function<void(aiNode*, SceneNode*)> addNode = [this, &addNode](aiNode* pAiNode, SceneNode* pParent)->void {
		nodes.emplace_back();
		SceneNode& node = nodes.back();
		if (pAiNode->mName.length != 0) {
			nodeNames[std::string(pAiNode->mName.C_Str())] = &node;
		}

		node.move(pParent);

		for (size_t i = 0; i < pAiNode->mNumMeshes; i++) {
			node.addMesh(&meshes[pAiNode->mMeshes[i]]);
		}

		for (size_t i = 0; i < pAiNode->mNumChildren; i++) {
			addNode(pAiNode->mChildren[i], &node);
		}
	};
	
	addNode(pAiScene->mRootNode, &root);
	
}

SceneNode* Scene::getRoot() {
	return &root;
}

SceneNode* Scene::getNode(const std::string& name) {
	auto it = nodeNames.find(name);
	if (it == nodeNames.end()) {
		return nullptr;
	}
	return it->second;
}

SceneNode* Scene::getNode(size_t index) {
	return &nodes[index];
}

Mesh* Scene::getMesh(const std::string& name) {
	auto it = meshNames.find(name);
	if (it == meshNames.end()) {
		return nullptr;
	}
	return it->second;
}

Mesh* Scene::getMesh(size_t index) {
	return &meshes[index];
}

void Scene::draw(Graphics& gfx) {
	root.draw(gfx, XMMatrixIdentity());
}
