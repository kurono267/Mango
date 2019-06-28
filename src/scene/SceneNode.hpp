//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENENODE_HPP
#define MANGO_SCENENODE_HPP

#include <unified/mesh.hpp>
#include "SceneTransform.hpp"
#include "Camera.hpp"
#include <vector>

namespace mango::scene {

class SceneNode : public SceneTransform, public std::enable_shared_from_this<SceneNode> {
	typedef std::shared_ptr<SceneNode> ptr;
	public:
		SceneNode();
		SceneNode(const spCamera& camera);
		SceneNode(const spMesh& geometry);
		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		ptr getParent();

		void setCamera(const spCamera& camera);
		void setGeometry(const spMesh& mesh);

		spMesh getGeometry();
		spCamera getCamera();

		glm::mat4 getWorldTransform();
	protected:
		std::vector<ptr> _childs;
		mutable ptr _parent;

		spCamera _camera;
		spMesh _geometry; // TODO made separeted class geometry
};

typedef std::shared_ptr<SceneNode> spSceneNode;

}


#endif //MANGO_SCENENODE_HPP
