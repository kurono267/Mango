//
// Created by kurono267 on 08.10.2020.
//

#include "Skin.hpp"

namespace mango {

Skin::Skin(const wpSceneNode &skeletonNode, const std::vector<wpSceneNode> &joints,
		   const std::vector<glm::mat4> &inverseBindMatrices)
		: _skeletonNode(skeletonNode), _joints(joints), _inverseBindMatrices(inverseBindMatrices) {}

wpSceneNode Skin::getSkeletonNode() const {
	return _skeletonNode;
}

const std::vector<wpSceneNode> &Skin::getJoints() const {
	return _joints;
}

const std::vector<glm::mat4> &Skin::getInveseBindMatrices() const {
	return _inverseBindMatrices;
}

std::shared_ptr<Skin> Skin::create(const wpSceneNode &skeletonNode, const std::vector<wpSceneNode> &joints,
								   const std::vector<glm::mat4> &inverseBindMatrices) {
	return std::make_shared<Skin>(skeletonNode, joints, inverseBindMatrices);
}

}
