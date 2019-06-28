//
// Created by kurono267 on 2019-06-26.
//

#include "SceneNode.hpp"

using namespace mango::scene;

SceneNode::SceneNode() {
	_parent = nullptr;
}

SceneNode::SceneNode(const spCamera &camera) {
	_parent = nullptr;
	_camera = camera;
}

SceneNode::SceneNode(const mango::spMesh &geometry) {
	_parent = nullptr;
	_geometry = geometry;
}

SceneNode::~SceneNode(){

}

void SceneNode::addChild(const SceneNode::ptr &child) {
	_childs.push_back(child);
	child->_parent = shared_from_this();
}

std::vector<SceneNode::ptr> &SceneNode::getChilds() {
	return _childs;
}

SceneNode::ptr SceneNode::getParent() {
	return _parent;
}

void SceneNode::setCamera(const spCamera &camera) {
	_camera = camera;
}

void SceneNode::setGeometry(const mango::spMesh &mesh) {
	_geometry = mesh;
}

mango::spMesh SceneNode::getGeometry() {
	return _geometry;
}

spCamera SceneNode::getCamera() {
	return _camera;
}

glm::mat4 SceneNode::getWorldTransform() {
	ptr rootNode = _parent;
	glm::mat4 transform = _transform;
	while(rootNode != nullptr){
		transform *= rootNode->transform();
		rootNode = rootNode->getParent();
	}
	return transform;
}
