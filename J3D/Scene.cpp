#include "Scene.h"
#include "Graphics.h"
#include "SceneLoading.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <json.hpp>

#include <algorithm>
#include <fstream>
#include <filesystem>
#include <cstddef>
#include <sstream>

using namespace DirectX;

SceneNode::SceneNode() : pParent(nullptr), pMesh(nullptr), transform() {
	XMStoreFloat4x4(&transform, XMMatrixIdentity());
}

void SceneNode::draw(Graphics& gfx, DirectX::FXMMATRIX parentTransform) {
	
	XMMATRIX accumulated = XMMatrixMultiply(parentTransform, XMLoadFloat4x4(&transform));

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

Scene::Scene(Graphics& gfx, const std::filesystem::path& file) :
	gfx(gfx) {
	
	using nlohmann::json;

	std::ifstream ifs(file);
	json j = json::parse(ifs);

	std::vector<GLTF::Buffer> buffers;
	std::vector<GLTF::BufferView> views;
	std::vector<GLTF::Accessor> accessors;

	json jbuffers = j.at("buffers");
	json jviews = j.at("bufferViews");
	json jaccessors = j.at("accessors");
	json jmeshes = j.at("meshes");

	for (auto& jbuffer : jbuffers) {
		std::filesystem::path bufferFile = file.parent_path();
		bufferFile.append(jbuffer.at("uri").get<std::string>());

		GLTF::Buffer buffer;
		buffer.size = jbuffer.at("byteLength").get<size_t>();
		buffer.stream = std::ifstream(bufferFile, std::ios::binary);

		buffers.emplace_back(std::move(buffer));
	}

	for (auto& jview : jviews) {
		GLTF::BufferView view;
		view.pBuffer = &buffers[jview.at("buffer").get<size_t>()];
		view.length = jview.at("byteLength").get<size_t>();
		view.offset = jview.contains("byteOffset") ? jview.at("byteOffset").get<size_t>() : 0;
		view.stride = jview.contains("byteStride") ? jview.at("byteStride").get<size_t>() : 0;

		view.length = jview.at("byteLength");
		views.emplace_back(std::move(view));
	}

	for (auto& jaccessor : jaccessors) {
		GLTF::Accessor accessor;
		accessor.pView = &views[jaccessor.at("bufferView").get<size_t>()];
		accessor.byteOffset = jaccessor.contains("byteOffset") ? jaccessor.at("byteOffset").get<size_t>() : 0;
		accessor.count = jaccessor.at("count");
		accessor.format = GLTF::getFormat(jaccessor.at("type").get<std::string>(), jaccessor.at("componentType").get<uint64_t>());

		accessors.emplace_back(std::move(accessor));
	}

	for (auto& jmesh : jmeshes) {

		DXUtils::VertexAttributes vertexAttribs;

		std::shared_ptr<VertexBuffer> pVB;
		std::shared_ptr<IndexBuffer> pIB;

		std::string name = jmesh.contains("name") ? jmesh.at("name").get<std::string>() : "";

		pVB = gfx.getBindableMgr().get<VertexBuffer>(name);
		pIB = gfx.getBindableMgr().get<IndexBuffer>(name);

		meshes.emplace_back();
		Mesh& mesh = meshes.back();
		if (name != "") meshNames[name] = &mesh;

		auto& jprimitive = jmesh.at("primitives").at(0);
		auto& jattributes = jprimitive.at("attributes");

		if (!pVB) {

			GLTF::Accessor* pPositionAccessor = &accessors[jattributes.at("POSITION").get<size_t>()];
			GLTF::Accessor* pNormalAccessor = jattributes.contains("NORMAL") ? &accessors[jattributes.at("NORMAL").get<size_t>()] : nullptr;
			GLTF::Accessor* pTangentAccessor = jattributes.contains("TANGENT") ? &accessors[jattributes.at("TANGENT").get<size_t>()] : nullptr;
			std::vector<GLTF::Accessor*> texcoordAccessors;
			std::vector<GLTF::Accessor*> colorAccessors;

			uint64_t i = 0;

			while (true) {
				std::stringstream ss("TEXCOORD_", std::ios_base::app | std::ios_base::out);
				ss << i;

				if (!jattributes.contains(ss.str())) {
					break;
				}

				GLTF::Accessor* pTexcoordAccessor = &accessors[jattributes.at(ss.str())];
				texcoordAccessors.push_back(pTexcoordAccessor);

				i++;
			}

			i = 0;

			while (true) {
				std::stringstream ss("COLOR_");
				ss << i;

				if (!jattributes.contains(ss.str())) {
					break;
				}

				GLTF::Accessor* pColorAccessor = &accessors[jattributes.at(ss.str())];
				colorAccessors.push_back(pColorAccessor);

				i++;
			}

			vertexAttribs.positionFormat = pPositionAccessor->format;

			if (pNormalAccessor) {
				vertexAttribs.normalFormat = pNormalAccessor->format;
			}
			if (pTangentAccessor) {
				vertexAttribs.tangentFormat = pTangentAccessor->format;
			}

			for (auto pTexcoordAccessor : texcoordAccessors) {
				vertexAttribs.texcoordFormats.push_back(pTexcoordAccessor->format);
			}

			for (auto pColorAccessor : colorAccessors) {
				vertexAttribs.colorFormats.push_back(pColorAccessor->format);
			}

			size_t vertexSize = vertexAttribs.getVertexSize();

			std::vector<std::byte> vertexData(pPositionAccessor->count * vertexSize);

			pPositionAccessor->copyTo(vertexData.data(), vertexAttribs.positionOffset(), vertexSize);
			if (pNormalAccessor) {
				pNormalAccessor->copyTo(vertexData.data(), vertexAttribs.normalOffset(), vertexSize);
			}
			if (pTangentAccessor) {
				pTangentAccessor->copyTo(vertexData.data(), vertexAttribs.tangentOffset(), vertexSize);
			}
			for (i = 0; i < texcoordAccessors.size(); i++) {
				texcoordAccessors[i]->copyTo(vertexData.data(), vertexAttribs.texcoordOffset(i), vertexSize);
			}
			for (i = 0; i < colorAccessors.size(); i++) {
				colorAccessors[i]->copyTo(vertexData.data(), vertexAttribs.colorOffset(i), vertexSize);
			}

			pVB = gfx.getBindableMgr().resolve<VertexBuffer>(name, vertexData.data(), vertexData.size(), vertexAttribs);
		}

		if (!pIB) {

			GLTF::Accessor* pIndexAccessor = &accessors[jprimitive.at("indices").get<size_t>()];
			std::vector<uint32_t> indices(pIndexAccessor->count);

			pIndexAccessor->copyTo(reinterpret_cast<std::byte*>(indices.data()), 0, sizeof(uint32_t));

			pIB = gfx.getBindableMgr().resolve<IndexBuffer>(name, indices);
		}

		mesh.addBindable(pVB);
		mesh.addBindable(pIB);
		mesh.addBindable(gfx.getBindableMgr().resolve<VertexShader>("./Shaders/VertexShader.cso", "./Shaders/VertexShader.cso"));
		mesh.addBindable(gfx.getBindableMgr().resolve<PixelShader>("./Shaders/PixelShader.cso", "./Shaders/PixelShader.cso"));
	}
	
	auto& jnodes = j.at("nodes");
	
	for (auto& jnode : jnodes) {
		nodes.emplace_back();
		if (jnode.contains("name")) {
			nodeNames[jnode.at("name").get<std::string>()] = &nodes.back();
		}
		if (jnode.contains("mesh")) {
			nodes.back().setMesh(&meshes[jnode.at("mesh").get<size_t>()]);
		}

	}

	// link nodes
	for (uint64_t i = 0; i < nodes.size(); i++) {
		auto& jnode = jnodes.at(i);
		if (jnode.contains("children")) {
			auto& childIndices = jnode.at("children");
			for (auto& childIndex : childIndices) {
				nodes[childIndex.get<size_t>()].move(&nodes[i]);
			}
		}
	}

	auto& jscene = j.at("scenes").at(0);
	if (jscene.contains("nodes")) {
		auto& nodeIndices = jscene.at("nodes");
		for (auto& nodeIndex : nodeIndices) {
			nodes[nodeIndex.get<size_t>()].move(&root);
		}
	}
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
