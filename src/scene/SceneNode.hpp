//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENENODE_HPP
#define MANGO_SCENENODE_HPP

#include <vector>

#include "SceneTransform.hpp"
#include "Camera.hpp"
#include "Geometry.hpp"
#include "BBox.hpp"
#include "../unified/Uniform.hpp"

namespace mango {

struct NodeData {
	glm::mat4 world;
};

struct CameraData {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewProj;
	glm::mat4 invView;
	glm::mat4 invProj;
};

class SceneNode : public SceneTransform,public std::enable_shared_from_this<SceneNode> {
	typedef std::shared_ptr<SceneNode> ptr;
	public:
		SceneNode();
		SceneNode(const spCamera& camera);
		SceneNode(const spGeometry& geometry);
		SceneNode(const spMesh& mesh, const spMaterial& material);
		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		SceneNode* getParent();

		void setCamera(const spCamera& camera);
		void setGeometry(const spGeometry& geometry);

		const spDescSet& getDescSet() const;

		BBox boundingBox();

		spGeometry getGeometry();
		spCamera getCamera();

		glm::mat4 getWorldTransform();

		void setTransform(const glm::mat4& transform) override;
		void setPos(const glm::vec3& pos) override;
		void setRotation(const glm::quat& quat) override;
		void setRotation(const glm::vec3& euler) override;
		void setScale(const glm::vec3& scale) override;

		void run(const std::function<void(const ptr& node, bool& stop)>& func,bool isRunForThis = true);

		static spDescSet generalDescSet();
	protected:
		void createDescSet();
		void updateDescSet();

		std::vector<ptr> _childs;
		mutable SceneNode* _parent;

		spCamera _camera;
		spGeometry _geometry;
		spDescSet _descSet;
		Uniform _uniform;
		NodeData _nodeData;
};

typedef std::shared_ptr<SceneNode> spSceneNode;

Uniform createCameraUniform(const spSceneNode& cameraNode = nullptr);
void updateCameraUniform(const spSceneNode& cameraNode, Uniform& uniform);

}


#endif //MANGO_SCENENODE_HPP
