#pragma once

#include "Mesh.h"

#include <vector>
#include <unordered_map>
#include <filesystem>

class SceneNode {
public:
	SceneNode();

	void move(SceneNode* pNewParent);
	void clear();

private:
	SceneNode* pParent;
	std::vector<SceneNode*> children;
	std::vector<Mesh*> meshes;
};

class Scene {
public:
	Scene(class Graphics& gfx, const std::filesystem::path& file);

	SceneNode* getRoot();
	SceneNode* getNode(const std::string& name);
	SceneNode* getNode(size_t index);
	Mesh* getMesh(const std::string& name);
	Mesh* getMesh(size_t index);

private:
	SceneNode root;
	std::vector<SceneNode> nodes;
	std::vector<Mesh> meshes;

	std::unordered_map<std::string, SceneNode*> nodeNames;
	std::unordered_map<std::string, Mesh*> meshNames;

	class Graphics& gfx;
};

