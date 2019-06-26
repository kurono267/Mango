//
// Created by kurono267 on 2019-06-26.
//

#include "SceneNode.hpp"

using namespace mango::scene;

SceneNode::SceneNode() {
	_parent = nullptr;
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
