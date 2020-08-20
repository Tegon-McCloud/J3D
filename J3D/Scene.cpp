#include "Scene.h"
#include "Graphics.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Material.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstddef>
#include <sstream>

using namespace DirectX;

SceneNode::SceneNode() : pParent(nullptr), pMesh(nullptr), parentToThis() {
	XMStoreFloat4x4(&parentToThis, XMMatrixIdentity());
}

void SceneNode::draw(Graphics& gfx, DirectX::FXMMATRIX parentTransform) {
	
	XMMATRIX accumulated = XMMatrixMultiply(parentTransform, XMLoadFloat4x4(&parentToThis));

	for (auto pChild : children) {
		pChild->draw(gfx, accumulated);
	}

	if (pMesh) {
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

void SceneNode::setMesh(Mesh* pMesh) {
	this->pMesh = pMesh;
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

Scene::Scene(Graphics& gfx, const std::filesystem::path& file) :
	gfx(gfx) {
	
	Assimp::Importer imp;
	imp.ReadFile(file.generic_string(), aiProcess_JoinIdenticalVertices);
	aiScene* pAiScene = imp.GetOrphanedScene();

	for (size_t i = 0; i < pAiScene->mNumMeshes; i++) {
		meshes.emplace_back();
		Mesh& mesh = meshes.back();
		aiMesh* pAiMesh = pAiScene->mMeshes[i];

		DXUtils::VertexAttributes vertexAttribs;
		vertexAttribs.positionFormat = DXUtils::Format(DXUtils::AggregateType::VEC3, DXUtils::ComponentType::FLOAT);

		
		

	}

	
	



	//using nlohmann::json;

	//std::ifstream ifs(file);
	//const json j = json::parse(ifs);

	//std::vector<GLTF::Buffer> buffers;
	//std::vector<GLTF::BufferView> views;
	//std::vector<GLTF::Accessor> accessors;
	//std::vector<GLTF::Image> images;
	//std::vector<GLTF::Sampler> samplers;
	//std::vector<GLTF::Texture> textures;
	//std::vector<std::shared_ptr<Material>> materials;
	//
	//if (j.contains("buffers")) {
	//	const json& jbuffers = j.at("buffers");
	//	for (auto& jbuffer : jbuffers) {
	//		std::filesystem::path bufferFile = file.parent_path().append(jbuffer.at("uri").get<std::string>());

	//		GLTF::Buffer buffer;
	//		buffer.size = jbuffer.at("byteLength").get<size_t>();
	//		buffer.stream = std::ifstream(bufferFile, std::ios::binary);

	//		buffers.emplace_back(std::move(buffer));
	//	}
	//}
	//
	//if (j.contains("bufferViews")) {
	//	const json& jviews = j.at("bufferViews");
	//	for (auto& jview : jviews) {
	//		GLTF::BufferView view;
	//		view.pBuffer = &buffers[jview.at("buffer").get<size_t>()];
	//		view.length = jview.at("byteLength").get<size_t>();
	//		view.offset = jview.contains("byteOffset") ? jview.at("byteOffset").get<size_t>() : 0;
	//		view.stride = jview.contains("byteStride") ? jview.at("byteStride").get<size_t>() : 0;

	//		view.length = jview.at("byteLength");
	//		views.emplace_back(std::move(view));
	//	}
	//}

	//if (j.contains("accessors")) {
	//	const auto& jaccessors = j.at("accessors");
	//	for (auto& jaccessor : jaccessors) {
	//		GLTF::Accessor accessor;
	//		accessor.pView = &views[jaccessor.at("bufferView").get<size_t>()];
	//		accessor.byteOffset = jaccessor.contains("byteOffset") ? jaccessor.at("byteOffset").get<size_t>() : 0;
	//		accessor.count = jaccessor.at("count");
	//		accessor.format = GLTF::getFormat(jaccessor.at("type").get<std::string>(), jaccessor.at("componentType").get<uint64_t>());

	//		accessors.emplace_back(std::move(accessor));
	//	}
	//}

	//if (j.contains("images")) {
	//	const auto& jimages = j.at("images");
	//	for (auto& jimage : jimages) {
	//		GLTF::Image image;
	//		image.file = file.parent_path() / jimage.at("uri").get<std::string>();
	//		image.name = jimage.contains("name") ? jimage.at("name").get<std::string>() : jimage.at("uri").get<std::string>();
	//		images.push_back(image);
	//	}
	//}

	//if (j.contains("samplers")) {
	//	auto& jsamplers = j.at("samplers");
	//	for (auto& jsampler : jsamplers) {
	//		GLTF::Sampler sampler;
	//		sampler.uMode = jsampler.contains("wrapS") ? GLTF::getAddressMode(jsampler.at("wrapS").get<uint64_t>()) : D3D11_TEXTURE_ADDRESS_WRAP;
	//		sampler.vMode = jsampler.contains("wrapT") ? GLTF::getAddressMode(jsampler.at("wrapT").get<uint64_t>()) : D3D11_TEXTURE_ADDRESS_WRAP;
	//		samplers.push_back(sampler);
	//	}
	//}

	//if (j.contains("textures")) {
	//	const auto& jtextures = j.at("textures");
	//	for (auto& jtexture : jtextures) {
	//		GLTF::Texture texture;
	//		texture.pImage = &images[jtexture.at("source").get<size_t>()];
	//		if (jtexture.contains("sampler")) {
	//			texture.pSampler = &samplers[jtexture.at("sampler").get<size_t>()];
	//		}
	//		else {
	//			texture.pSampler = nullptr;
	//		}
	//		textures.push_back(texture);
	//	}
	//}

	//if (j.contains("materials")) {
	//	const auto& jmaterials = j.at("materials");
	//	
	//	for (auto& jmaterial : jmaterials) {
	//		

	//		std::shared_ptr<Material> pMaterial;
	//		MaterialConstants materialConstants;

	//		if (jmaterial.contains("pbrMetallicRoughness")) {
	//			const auto& jpbr = jmaterial.at("pbrMetallicRoughness");
	//			
	//			if (jpbr.contains("baseColorFactor")) {
	//				const auto& jbaseColor = jpbr.at("baseColorFactor");
	//				
	//				for (size_t i = 0; i < 4; i++) {
	//					materialConstants.color[i] = jbaseColor.at(i).get<float>();
	//				}
	//			} else {
	//				for (size_t i = 0; i < 4; i++) {
	//					materialConstants.color[i] = 1.0f;
	//				}
	//			}
	//			
	//			materialConstants.metallic = jpbr.contains("metallic") ? jpbr.at("metallic").get<float>() : 1.0f;
	//			materialConstants.roughness = jpbr.contains("roughness") ? jpbr.at("roughness").get<float>() : 1.0f;
	//			
	//			pMaterial = gfx.getResourceMgr<Material>().resolve("", materialConstants);

	//			if (jpbr.contains("baseColorTexture")) {
	//				GLTF::Texture* pGLTFTexture = &textures[jpbr.at("baseColorTexture").get<size_t>()];
	//				std::shared_ptr<PSTexture2D> pTexture = gfx.getResourceMgr<PSTexture2D>().resolve(pGLTFTexture->pImage->name, pGLTFTexture->pImage->file);
	//				std::shared_ptr<PSSampler> pSampler;
	//				if (pGLTFTexture->pSampler) {
	//					pSampler = gfx.getResourceMgr<PSSampler>().resolve("", pGLTFTexture->pSampler->uMode, pGLTFTexture->pSampler->vMode);
	//				} else {
	//					pSampler = gfx.getResourceMgr<PSSampler>().resolve("default", D3D10_TEXTURE_ADDRESS_WRAP, D3D10_TEXTURE_ADDRESS_WRAP);
	//				}
	//				pMaterial->setColorMap(pTexture, pSampler);
	//			}
	//		}


	//	}
	//}


	//if (j.contains("meshes")) {
	//	const json& jmeshes = j.at("meshes");
	//	for (auto& jmesh : jmeshes) {

	//		DXUtils::VertexAttributes vertexAttribs;

	//		std::shared_ptr<VertexBuffer> pVB;
	//		std::shared_ptr<IndexBuffer> pIB;

	//		std::string name = jmesh.contains("name") ? jmesh.at("name").get<std::string>() : "";

	//		pVB = gfx.getResourceMgr<VertexBuffer>().get(name);
	//		pIB = gfx.getResourceMgr<IndexBuffer>().get(name);

	//		meshes.emplace_back();
	//		Mesh& mesh = meshes.back();
	//		if (name != "") meshNames[name] = &mesh;

	//		auto& jprimitive = jmesh.at("primitives").at(0);
	//		auto& jattributes = jprimitive.at("attributes");

	//		if (!pVB) {
	//			GLTF::Accessor* pPositionAccessor = &accessors[jattributes.at("POSITION").get<size_t>()];
	//			GLTF::Accessor* pNormalAccessor = jattributes.contains("NORMAL") ? &accessors[jattributes.at("NORMAL").get<size_t>()] : nullptr;
	//			GLTF::Accessor* pTangentAccessor = jattributes.contains("TANGENT") ? &accessors[jattributes.at("TANGENT").get<size_t>()] : nullptr;
	//			std::vector<GLTF::Accessor*> texcoordAccessors;
	//			std::vector<GLTF::Accessor*> colorAccessors;

	//			uint64_t i = 0;

	//			while (true) {
	//				std::stringstream ss("TEXCOORD_", std::ios_base::app | std::ios_base::out);
	//				ss << i;

	//				if (!jattributes.contains(ss.str())) {
	//					break;
	//				}

	//				GLTF::Accessor* pTexcoordAccessor = &accessors[jattributes.at(ss.str())];
	//				texcoordAccessors.push_back(pTexcoordAccessor);

	//				i++;
	//			}

	//			i = 0;

	//			while (true) {
	//				std::stringstream ss("COLOR_");
	//				ss << i;

	//				if (!jattributes.contains(ss.str())) {
	//					break;
	//				}

	//				GLTF::Accessor* pColorAccessor = &accessors[jattributes.at(ss.str())];
	//				colorAccessors.push_back(pColorAccessor);

	//				i++;
	//			}

	//			vertexAttribs.positionFormat = pPositionAccessor->format;

	//			if (pNormalAccessor) {
	//				vertexAttribs.normalFormat = pNormalAccessor->format;
	//			}
	//			if (pTangentAccessor) {
	//				vertexAttribs.tangentFormat = pTangentAccessor->format;
	//			}

	//			for (auto pTexcoordAccessor : texcoordAccessors) {
	//				vertexAttribs.texcoordFormats.push_back(pTexcoordAccessor->format);
	//			}

	//			for (auto pColorAccessor : colorAccessors) {
	//				vertexAttribs.colorFormats.push_back(pColorAccessor->format);
	//			}

	//			size_t vertexSize = vertexAttribs.getVertexSize();

	//			std::vector<std::byte> vertexData(pPositionAccessor->count * vertexSize);

	//			pPositionAccessor->copyTo(vertexData.data(), vertexAttribs.positionOffset(), vertexSize);
	//			if (pNormalAccessor) {
	//				pNormalAccessor->copyTo(vertexData.data(), vertexAttribs.normalOffset(), vertexSize);
	//			}
	//			if (pTangentAccessor) {
	//				pTangentAccessor->copyTo(vertexData.data(), vertexAttribs.tangentOffset(), vertexSize);
	//			}
	//			for (i = 0; i < texcoordAccessors.size(); i++) {
	//				texcoordAccessors[i]->copyTo(vertexData.data(), vertexAttribs.texcoordOffset(i), vertexSize);
	//			}
	//			for (i = 0; i < colorAccessors.size(); i++) {
	//				colorAccessors[i]->copyTo(vertexData.data(), vertexAttribs.colorOffset(i), vertexSize);
	//			}

	//			pVB = gfx.getResourceMgr<VertexBuffer>().resolve(name, vertexData.data(), vertexData.size(), vertexAttribs);
	//		}

	//		if (!pIB) {

	//			GLTF::Accessor* pIndexAccessor = &accessors[jprimitive.at("indices").get<size_t>()];
	//			std::vector<uint32_t> indices(pIndexAccessor->count);

	//			pIndexAccessor->copyTo(reinterpret_cast<std::byte*>(indices.data()), 0, sizeof(uint32_t));

	//			pIB = gfx.getResourceMgr<IndexBuffer>().resolve(name, indices);
	//		}

	//		mesh.addBindable(pVB);
	//		mesh.addBindable(pIB);
	//		mesh.addBindable(gfx.getResourceMgr<VertexShader>().resolve("VS",
	//			std::filesystem::path("./VertexShader.hlsl")));
	//		mesh.addBindable(gfx.getResourceMgr<PixelShader>().resolve("PSColorMap",
	//			std::filesystem::path("./PixelShader.hlsl"), true, false));
	//		
	//		if (jprimitive.contains("material")) {
	//			const auto& jmaterial = jprimitive.at("material");
	//			
	//			
	//		}
	//	}
	//}

	//
	//if (j.contains("nodes")) {
	//	auto& jnodes = j.at("nodes");
	//	for (auto& jnode : jnodes) {
	//		nodes.emplace_back();
	//		if (jnode.contains("name")) {
	//			nodeNames[jnode.at("name").get<std::string>()] = &nodes.back();
	//		}
	//		if (jnode.contains("mesh")) {
	//			nodes.back().setMesh(&meshes[jnode.at("mesh").get<size_t>()]);
	//		}

	//	}

	//	// link nodes
	//	for (uint64_t i = 0; i < nodes.size(); i++) {
	//		auto& jnode = jnodes.at(i);
	//		if (jnode.contains("children")) {
	//			auto& childIndices = jnode.at("children");
	//			for (auto& childIndex : childIndices) {
	//				nodes[childIndex.get<size_t>()].move(&nodes[i]);
	//			}
	//		}
	//	}
	//}
	//if (j.contains("scenes") && j.at("scenes").size() > 0) {
	//	auto& jscene = j.at("scenes").at(0);
	//	if (jscene.contains("nodes")) {
	//		auto& nodeIndices = jscene.at("nodes");
	//		for (auto& nodeIndex : nodeIndices) {
	//			nodes[nodeIndex.get<size_t>()].move(&root);
	//		}
	//	}
	//}
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
