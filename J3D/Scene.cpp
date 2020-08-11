#include "Scene.h"

SceneNode::SceneNode() : pParent(nullptr) {}

void SceneNode::move(SceneNode* pNewParent) {
	if (pParent) {
		auto it = std::find(pParent->children.begin(), pParent->children.end(), this);
		if (it != pParent->children.end()) {
			pParent->children.erase(it);
		}
	}
}

Scene::Scene() {}

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


