#include "Scene.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"
#include "PBRMaterial.h"
#include "PhongMaterial.h"

#include <assimp/DefaultLogger.hpp>
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
	const aiScene* pAiScene = imp.ReadFile(file.generic_string(), aiProcess_JoinIdenticalVertices | aiProcess_Triangulate);
	
	if (!pAiScene) {
		throw std::runtime_error(std::string("File was not found or was invalid: ") + file.string());
	}

	std::vector<std::shared_ptr<Material>> materials;

	for (size_t i = 0; i < pAiScene->mNumMaterials; i++) {
		aiMaterial& aiMat = *pAiScene->mMaterials[i];

		std::shared_ptr<Material> pMaterial;

		if (PBR::Material::isCompatible(aiMat)) {
			pMaterial = gfx.getResourceMgr<PBR::Material>().resolve("", file, aiMat);
		} else if (Phong::Material::isCompatible(aiMat)) {
			
		} else {
			throw std::runtime_error("unsupported material");
		}

		materials.push_back(pMaterial);

	}


	for (size_t i = 0; i < pAiScene->mNumMeshes; i++) {
		meshes.emplace_back();
		Mesh& mesh = meshes.back();
		aiMesh& aiMesh = *pAiScene->mMeshes[i];

		DXUtils::VertexAttributes vertexAttribs;
		vertexAttribs.positionFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.normalFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.tangentFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);
		vertexAttribs.texcoordFormats.push_back(DXUtils::Format(DXUtils::AggregateType::VEC2, DXUtils::ComponentType::FLOAT));
		
		size_t vertexSize = vertexAttribs.getVertexSize();
		
		std::vector<std::byte> vertexData(aiMesh.mNumVertices * vertexSize);

		for (size_t i = 0; i < aiMesh.mNumVertices; i++) {
			size_t offset = vertexSize * i;
			float* pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.positionOffset()]);
			pData[0] = aiMesh.mVertices[i].x;
			pData[1] = aiMesh.mVertices[i].y;
			pData[2] = aiMesh.mVertices[i].z;

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.normalOffset()]);
			pData[0] = aiMesh.mNormals[i].x;
			pData[1] = aiMesh.mNormals[i].y;
			pData[2] = aiMesh.mNormals[i].z;

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.tangentOffset()]);
			pData[0] = aiMesh.mTangents[i].x;
			pData[1] = aiMesh.mTangents[i].y;
			pData[2] = aiMesh.mTangents[i].z;

			assert(aiMesh.GetNumUVChannels() > 0);
			assert(aiMesh.mNumUVComponents[0] == 2);

			pData = reinterpret_cast<float*>(&vertexData[offset + vertexAttribs.texcoordOffset(0)]);
			pData[0] = aiMesh.mTextureCoords[0][i].x;
			pData[1] = aiMesh.mTextureCoords[0][i].y;
		}
		
		std::shared_ptr<VertexBuffer> pVB = gfx.getResourceMgr<VertexBuffer>().resolve("", vertexData, vertexAttribs);
		mesh.addBindable(std::move(pVB));
		
		std::vector<uint32_t> indexData(static_cast<size_t>(aiMesh.mNumFaces) * 3);

		for (size_t i = 0; i < aiMesh.mNumFaces; i++) {
			assert(aiMesh.mFaces[i].mNumIndices == 3);

			indexData.push_back(aiMesh.mFaces[i].mIndices[0]);
			indexData.push_back(aiMesh.mFaces[i].mIndices[1]);
			indexData.push_back(aiMesh.mFaces[i].mIndices[2]);
		}
		
		std::shared_ptr<IndexBuffer> pIB = gfx.getResourceMgr<IndexBuffer>().resolve("", indexData);
		mesh.addBindable(pIB);

		std::shared_ptr<Material> pMat = materials[aiMesh.mMaterialIndex];
		mesh.addBindable(pMat);

		std::shared_ptr<VertexShader> pVS = gfx.getResourceMgr<VertexShader>().resolve(
			"", std::filesystem::path("./VS.hlsl"));
		mesh.addBindable(pVS);

		std::shared_ptr<PixelShader> pPS = gfx.getResourceMgr<PixelShader>().resolve(
			"", pMat);
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
