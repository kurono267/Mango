//
// Created by kurono267 on 2019-06-26.
//

#include "SceneNode.hpp"

using namespace mango;

SceneNode::SceneNode() {
	_parent = nullptr;
}

SceneNode::SceneNode(const spCamera &camera) {
	_parent = nullptr;
	_camera = camera;
}

SceneNode::SceneNode(const mango::spGeometry &geometry) {
	_parent = nullptr;
	_geometry = geometry;
}

SceneNode::SceneNode(const spMesh &mesh, const spMaterial &material) {
	_parent = nullptr;
	_geometry = Geometry::make(mesh,material);
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

void SceneNode::setGeometry(const spGeometry &geometry) {
	_geometry = geometry;
}

spGeometry SceneNode::getGeometry() {
	return _geometry;
}

spCamera SceneNode::getCamera() {
	return _camera;
}

glm::mat4 SceneNode::getWorldTransform() {
	if(!_parent)return _transform;
	glm::mat4 transform = _transform*_parent->getWorldTransform();
	return transform;
}

spDescSet SceneNode::getDescSet(const spDevice& device) {
	if(!_descSet){
		_descSet = device->createDescSet();
	}
}

void SceneNode::setTransform(const glm::mat4& transform) {
	SceneTransform::setTransform(transform);

}

void SceneNode::setPos(const glm::vec3& pos) {
	SceneTransform::setPos(pos);

}

void SceneNode::setRotation(const glm::quat& quat) {
	SceneTransform::setRotation(quat);

}

void SceneNode::setRotation(const glm::vec3& euler) {
	SceneTransform::setRotation(euler);

}

void SceneNode::setScale(const glm::vec3& scale) {
	SceneTransform::setRotation(scale);

}

void SceneNode::run(const std::function<void(const ptr &, bool &)> &func, bool isRunForThis) {
	bool stop = false;
	if(isRunForThis)func(shared_from_this(),stop);
	if(stop)return;
	for(auto child : _childs){
		func(child,stop);
		if(stop)return;
		child->run(func,false);
	}
}

BBox SceneNode::boundingBox() {
	BBox box;
	if(_geometry && _geometry->getMesh()){
		box.expand(_geometry->getMesh()->getBoundingBox());
	}
	for(auto& child : _childs){
		auto childBox = child->boundingBox();
		childBox = childBox.applyTransform(child->transform());
		box.expand(child->boundingBox());
	}
	return box;
}
