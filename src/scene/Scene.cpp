//
// Created by kurono267 on 2019-07-29.
//

#include "Scene.hpp"

using namespace mango;

Scene::Scene(const spSceneNode& node) : rootNode(node) {}
Scene::Scene() {
	rootNode = SceneNode::create();
}

spSceneNode Scene::getCameraNode() const {
	spSceneNode finded;
	rootNode->run([&finded](const spSceneNode& node, bool& isStop){
		if(node->getCamera() != nullptr){
			finded = node;
			isStop = true;
		}
	});
	return finded;
}

void Scene::setUpdated(bool isUpdated) {
	rootNode->run([isUpdated](const spSceneNode& node, bool& isStop){
		node->setUpdated(isUpdated);
	});
}

bool Scene::getUpdated() {
	bool isUpdated = false;
	rootNode->run([&isUpdated](const spSceneNode& node, bool& isStop){
		isUpdated = node->isUpdated();
		isStop = isUpdated;
	});
	return isUpdated;
}

std::shared_ptr<Scene> Scene::create() {
	return std::make_shared<Scene>();
}
