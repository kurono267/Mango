//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENENODE_HPP
#define MANGO_SCENENODE_HPP

#include "SceneTransform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include <vector>

namespace mango {

class SceneNode : public SceneTransform, public std::enable_shared_from_this<SceneNode> {
	typedef std::shared_ptr<SceneNode> ptr;
	public:
		SceneNode();
		SceneNode(const spCamera& camera);
		SceneNode(const spGeometry& geometry);
		SceneNode(const spMesh& mesh, const spMaterial& material);
		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		ptr getParent();

		void setCamera(const spCamera& camera);
		void setGeometry(const spGeometry& geometry);

		spGeometry getGeometry();
		spCamera getCamera();

		glm::mat4 getWorldTransform();
	protected:
		std::vector<ptr> _childs;
		mutable ptr _parent;

		spCamera _camera;
		spGeometry _geometry;
};

typedef std::shared_ptr<SceneNode> spSceneNode;

}


#endif //MANGO_SCENENODE_HPP
