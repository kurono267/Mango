//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_SCENE_HPP
#define MANGO_SCENE_HPP

#include "SceneNode.hpp"

namespace mango {

class Scene {
	public:
		Scene() = default;
		Scene(const spSceneNode& node);

		spSceneNode getCameraNode() const;

		spTexture background;
		spTexture environmentLight;
		spSceneNode rootNode;
};

}

#endif //MANGO_SCENE_HPP
