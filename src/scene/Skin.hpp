//
// Created by kurono267 on 08.10.2020.
//

#ifndef MANGOWORLD_SKIN_HPP
#define MANGOWORLD_SKIN_HPP

#include <memory>
#include <vector>
#include "api/default.hpp"

namespace mango {

class SceneNode;

typedef std::weak_ptr<SceneNode> wpSceneNode;

class Skin {
	public:
		Skin(const wpSceneNode &skeletonNode, const std::vector<wpSceneNode> &joints,
			 const std::vector<glm::mat4> &inverseBindMatrices);

		wpSceneNode getSkeletonNode() const;

		const std::vector<wpSceneNode> &getJoints() const;

		const std::vector<glm::mat4> &getInveseBindMatrices() const;

		static std::shared_ptr<Skin> create(const wpSceneNode &skeletonNode, const std::vector<wpSceneNode> &joints,
											const std::vector<glm::mat4> &inverseBindMatrices);

	protected:
		wpSceneNode _skeletonNode;
		std::vector<wpSceneNode> _joints;
		std::vector<glm::mat4> _inverseBindMatrices;
};

typedef std::shared_ptr<Skin> spSkin;

}

#endif //MANGOWORLD_SKIN_HPP
