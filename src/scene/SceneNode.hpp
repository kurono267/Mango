//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENENODE_HPP
#define MANGO_SCENENODE_HPP

#include <vector>

#include "SceneTransform.hpp"
#include "Camera.hpp"
#include "Renderable.hpp"
#include "BBox.hpp"
#include "../unified/Uniform.hpp"
#include "Light.hpp"
#include "Context.hpp"

namespace mango {

struct NodeData {
	glm::mat4 world; // 64 bytes
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
		SceneNode() = default;
		SceneNode(const spEntity& entity);

		static std::shared_ptr<SceneNode> create();
		static std::shared_ptr<SceneNode> create(const spLight& light);
		static std::shared_ptr<SceneNode> create(const spCamera& camera);
		static std::shared_ptr<SceneNode> create(const spRenderable& rendererable);
		static std::shared_ptr<SceneNode> create(const spMesh& mesh, const spMaterial& material = nullptr);

		~SceneNode();

		void addChild(const ptr& child);
		std::vector<ptr>& getChilds();

		void setEntity(const spEntity& entity);
		spEntity getEntity();

		SceneNode* getParent();

		void setName(const std::string& name);
		std::string getName();

		BBox boundingBox();
		glm::mat4 getWorldTransform();

		void run(const std::function<void(const ptr& node, bool& stop)>& func,bool isRunForThis = true);

		void setUpdated(bool isUpdated) override;
	protected:
		std::vector<ptr> _childs;
		mutable SceneNode* _parent = nullptr;

		spEntity _entity = nullptr;

		uint32_t _nodeIndex = 0;
		std::string _name;
};

typedef std::shared_ptr<SceneNode> spSceneNode;

Uniform createCameraUniform(const spSceneNode& cameraNode = nullptr);
void updateCameraUniform(const spSceneNode& cameraNode, Uniform& uniform);

}


#endif //MANGO_SCENENODE_HPP
