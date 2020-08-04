//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_SCENE_HPP
#define MANGO_SCENE_HPP

#include "SceneNode.hpp"

namespace mango {

class Scene {
	public:
		Scene();
		Scene(const spSceneNode& node);

		spSceneNode getCameraNode() const;

		spTexture background;
		spTexture environmentLight;
		spSceneNode rootNode;

		static std::shared_ptr<Scene> create();
};

typedef std::shared_ptr<Scene> spScene;

}

#endif //MANGO_SCENE_HPP
