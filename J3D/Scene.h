#pragma once

#include <vector>

class SceneNode {
public:
	SceneNode() = default;


private:
	SceneNode* pParent;
	std::vector<SceneNode*> children;

};

class Scene {
public:
	Scene();

private:
	SceneNode root;
	
};

