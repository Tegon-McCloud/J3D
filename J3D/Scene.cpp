#include "Scene.h"
#include "Graphics.h"
#include "SceneLoading.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

#include "json.hpp"

#include <algorithm>
#include <fstream>
#include <filesystem>

using nlohmann::json;

SceneNode::SceneNode() : pParent(nullptr) {}

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

Scene::Scene(Graphics& gfx, const std::filesystem::path& file) :
	gfx(gfx) {
	
	std::ifstream fs(file);
	json j = nlohmann::json::parse(fs);

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
		buffer.stream = std::ifstream(bufferFile);

		buffers.emplace_back(std::move(buffer));
	}

	for (auto& jview : jviews) {
		GLTF::BufferView view;
		view.pBuffer = &buffers[jview.at("buffer").get<size_t>()];
		view.length = jview.at("byteLength").get<size_t>();
		view.offset = jview.contains("byteOffset") ? jview.at("byteOffset").get<size_t>() : 0;
		view.stride = jview.contains("byteStride") ? jview.at("byteStride").get<size_t>() : 0;
		
		view.length = jview.at("byteLength");
	}

	for (auto& jaccessor : jaccessors) {
		GLTF::Accessor accessor;
		accessor.pView = &views[jaccessor.at("bufferView").get<size_t>()];
		accessor.byteOffset = jaccessor.contains("byteOffset") ? jaccessor.at("byteOffset").get<size_t>() : 0;
		accessor.count = jaccessor.at("count");
	}

	for (auto& jmesh : jmeshes) {
		
		meshes.emplace_back();
		Mesh* pMesh = &meshes.back();

		std::shared_ptr<VertexBuffer> pVB;
		std::shared_ptr<IndexBuffer> pIB;


		if (jmesh.contains("name")) {
			meshNames[jmesh.at("name")] = pMesh;
		}

		
		auto& jprimitive = jmesh.at("primitives").at(0);
		auto& jattributes = jprimitive.at("attributes");

		for (auto& jattribute : jattributes) {

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
