//
// Created by kurono267 on 2019-06-26.
//

#include "SceneNode.hpp"
#include "../api/Instance.hpp"
#include "../api/DescSet.hpp"

using namespace mango;

SceneNode::SceneNode() {
	_parent = nullptr;
	_nodeData.world = glm::mat4(1.f);
}

SceneNode::SceneNode(const spCamera &camera) {
	_parent = nullptr;
	_camera = camera;
}

SceneNode::SceneNode(const mango::spGeometry &geometry) {
	_parent = nullptr;
	_geometry = geometry;
	_nodeData.world = glm::mat4(1.f);
	createDescSet();
}

SceneNode::SceneNode(const spMesh &mesh, const spMaterial &material) {
	_parent = nullptr;
	_geometry = Geometry::create(mesh,material);
	_nodeData.world = glm::mat4(1.f);
	createDescSet();
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
}

std::vector<SceneNode::ptr> &SceneNode::getChilds() {
	return _childs;
}

SceneNode* SceneNode::getParent() {
	return _parent;
}

void SceneNode::setCamera(const spCamera &camera) {
	_camera = camera;
}

void SceneNode::setGeometry(const spGeometry &geometry) {
	_geometry = geometry;
	if(!_descSet){
		createDescSet();
	}
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

const spDescSet& SceneNode::getDescSet() const {
	return _descSet;
}

void SceneNode::setTransform(const glm::mat4& transform) {
	SceneTransform::setTransform(transform);
	updateDescSet();
}

void SceneNode::setPos(const glm::vec3& pos) {
	SceneTransform::setPos(pos);
	updateDescSet();
}

void SceneNode::setRotation(const glm::quat& quat) {
	SceneTransform::setRotation(quat);
	updateDescSet();
}

void SceneNode::setRotation(const glm::vec3& euler) {
	SceneTransform::setRotation(euler);
	updateDescSet();
}

void SceneNode::setScale(const glm::vec3& scale) {
	SceneTransform::setScale(scale);
	updateDescSet();
}

void SceneNode::updateDescSet(){
	if(!_descSet && _geometry)createDescSet();
	if(_descSet){
		_nodeData.world = getWorldTransform();
		_uniform.set(sizeof(NodeData),&_nodeData);
	}
}

void SceneNode::createDescSet() {
	auto device = Instance::device<Device>();
	_descSet = device->createDescSet();
	_uniform.create(device,sizeof(NodeData),&_nodeData);
	_descSet->setUniformBuffer(_uniform,0,ShaderStage::Vertex);
	_descSet->create();
}

spDescSet SceneNode::generalDescSet() {
	auto device = Instance::device<Device>();
	auto descSet = device->createDescSet();
	Uniform uniform;
	uniform.create(device,sizeof(NodeData));
	descSet->setUniformBuffer(uniform,0,ShaderStage::Vertex);
	descSet->create();
	return descSet;
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

Uniform mango::createCameraUniform(const spSceneNode& cameraNode) {
	auto device = Instance::device();

	Uniform cameraUniform;
	cameraUniform.create(device,sizeof(CameraData));
	if(cameraNode)updateCameraUniform(cameraNode,cameraUniform);
	return cameraUniform;
}

void mango::updateCameraUniform(const spSceneNode& cameraNode, Uniform& uniform) {
	if(!cameraNode->getCamera())throw std::logic_error("updateCameraUniform: Node don't contains camera");
	CameraData data;
	data.view = cameraNode->getWorldTransform();
	data.proj = cameraNode->getCamera()->getProj();
	data.viewProj = data.proj*data.view;
	data.invView = glm::inverse(data.view);
	data.invProj = glm::inverse(data.proj);

	uniform.set(sizeof(CameraData),&data);
}
