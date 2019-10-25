//
// Created by kurono267 on 2019-09-05.
//

#ifndef MANGO_RAYTRACER_HPP
#define MANGO_RAYTRACER_HPP

#include <api/Device.hpp>
#include <scene/SceneNode.hpp>
#include <scene/Scene.hpp>
#include <scene/BVH.hpp>
#include <unified/Uniform.hpp>

namespace mango {

class Raytracer {
	public:
		Raytracer(const glm::ivec2& size);

		void buildTree(const spSceneNode& sceneNode);

		void render(const Scene& scene, const spSemaphore& wait, const spSemaphore& finish);

		spTextureView getPos() const;
		spTextureView getNormal() const;
		spTextureView getAlbedo() const;
		spTextureView getMaterial() const;
		// Only for testing
		spTexture getCameraOrigin() const;
		spTexture getCameraDir() const;
	private:
		std::vector<spBVH> _trees;
		std::vector<spSceneNode> _nodes;
		std::vector<spDescSet> _nodeDescSets;

		spTextureView _pos;
		spTextureView _normal;
		spTextureView _albedo;
		spTextureView _material;

		spCompute _compute;
		spCommandBuffer _commandBuffer;
		spDescSet _output;
		spDescSet _input;

		spMesh _quad;

		Uniform _uniform;

		// Rendering from camera for testing
		spPipeline _cameraPipeline;
		spRenderPass _cameraRenderPass;
		spFramebuffer _cameraFramebuffer;
		spDescSet _cameraDescSet;
		spMesh _quadMesh;
		spTexture _cameraOrigin;
		spTexture _cameraDir;
		Uniform _cameraUniform;
		spCommandBuffer _cameraCommandBuffer;
		spSemaphore _cameraSemaphore;
	private:
		void createCameraPipeline(const glm::ivec2& size);
};

typedef std::shared_ptr<Raytracer> spRaytracer;

}

#endif //MANGO_RAYTRACER_HPP
