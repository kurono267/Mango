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
	glm::mat4 world; // 64 bytes
	uint32_t alignment[48]; // 256-64
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

		static std::shared_ptr<SceneNode> create();
		static std::shared_ptr<SceneNode> create(const spCamera& camera);
		static std::shared_ptr<SceneNode> create(const spGeometry& geometry);
		static std::shared_ptr<SceneNode> create(const spMesh& mesh, const spMaterial& material = nullptr);

		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		void setRenderType(uint32_t renderType);
		uint32_t getRenderType();

		SceneNode* getParent();

		void setCamera(const spCamera& camera);
		void setGeometry(const spGeometry& geometry);

		BBox boundingBox();

		spGeometry getGeometry();
		spCamera getCamera();

		glm::mat4 getWorldTransform();

		void run(const std::function<void(const ptr& node, bool& stop)>& func,bool isRunForThis = true);
	protected:
		std::vector<ptr> _childs;
		mutable SceneNode* _parent;

		spCamera _camera;
		spGeometry _geometry;

		uint32_t _renderType = 0;
};

typedef std::shared_ptr<SceneNode> spSceneNode;

Uniform createCameraUniform(const spSceneNode& cameraNode = nullptr);
void updateCameraUniform(const spSceneNode& cameraNode, Uniform& uniform);

}


#endif //MANGO_SCENENODE_HPP
