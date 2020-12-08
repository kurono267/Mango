//
// Created by kurono267 on 2019-06-26.
//

#include "SceneNode.hpp"
#include "../api/Instance.hpp"
#include "../api/DescSet.hpp"
#include "../unified/Timer.hpp"

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
	_geometry = Geometry::create(mesh,material);
}

std::shared_ptr<SceneNode> SceneNode::create() {
	return std::make_shared<SceneNode>();
}

std::shared_ptr<SceneNode> SceneNode::create(const spCamera& camera) {
	return std::make_shared<SceneNode>(camera);
}

std::shared_ptr<SceneNode> SceneNode::create(const spGeometry& geometry){
	return std::make_shared<SceneNode>(geometry);
}

std::shared_ptr<SceneNode> SceneNode::create(const spMesh& mesh, const spMaterial& material) {
	return std::make_shared<SceneNode>(mesh,material);
}

SceneNode::~SceneNode(){

}

void SceneNode::addChild(const SceneNode::ptr &child) {
	_childs.push_back(child);
	child->_parent = this;
	setUpdated(true);
}

std::vector<SceneNode::ptr> &SceneNode::getChilds() {
	return _childs;
}

void SceneNode::setUpdated(bool isUpdated) {
	_isUpdated = isUpdated;
	for(int i = 0;i<_childs.size();++i){
		auto child = _childs[i];
		child->setUpdated(_isUpdated);
	}
}

SceneNode* SceneNode::getParent() {
	return _parent;
}

void SceneNode::setRenderType(uint32_t renderType) {
	_renderType = renderType;
}

uint32_t SceneNode::getRenderType() {
	return _renderType;
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

void SceneNode::setName(const std::string& name) {
	_name = name;
}
std::string SceneNode::getName() {
	return _name;
}

spCamera SceneNode::getCamera() {
	return _camera;
}

glm::mat4 SceneNode::getWorldTransform() {
	auto t = _transform;
	auto p = _parent;
	while(p){
		t = p->transform()*t;
		p = p->getParent();
	}
	return t;
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
		childBox = childBox.applyTransform(getWorldTransform());
		box.expand(child->boundingBox());
	}
	return box;
}

Uniform mango::createCameraUniform(const spSceneNode& cameraNode) {
	auto device = Instance::device();

	Uniform cameraUniform;
	cameraUniform.create(sizeof(CameraData),BufferType::Uniform);
	if(cameraNode)updateCameraUniform(cameraNode,cameraUniform);
	return cameraUniform;
}

void mango::updateCameraUniform(const spSceneNode& cameraNode, Uniform& uniform) {
	if(!cameraNode->getCamera())throw std::logic_error("updateCameraUniform: Node don't contains camera");
	CameraData data;
	auto camera2world = cameraNode->getWorldTransform();
	auto world2camera = glm::inverse(camera2world);
	auto proj = cameraNode->getCamera()->getProj();
	data.view = world2camera;
	data.proj = proj;
	data.viewProj = proj*world2camera;
	data.invView = camera2world;
	data.invProj = glm::inverse(proj);

	uniform.set(sizeof(CameraData),&data);
}
