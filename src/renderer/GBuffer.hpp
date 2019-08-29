//
// Created by kurono267 on 2019-08-21.
//

#ifndef MANGO_GBUFFER_HPP
#define MANGO_GBUFFER_HPP

#include <api/Device.hpp>
#include <scene/SceneNode.hpp>
#include <scene/Scene.hpp>
#include <unified/Uniform.hpp>

namespace mango {

class GBuffer {
	public:
		GBuffer(const spDevice &device, const glm::ivec2& size);

		void update(const spSceneNode &scene);

		void render(const Scene& sceneNode, const spSemaphore& wait, const spSemaphore& finish);

		spTexture getAlbedo() const;
		spTexture getNormal() const;
		spTexture getPos() const;
	protected:
		spDevice _device;

		Uniform _cameraUniform;
		Uniform _nodeUniform;

		std::vector<spDescSet> _descSets;



		spCommandBuffer _commandBuffer;
		spPipeline _pipeline;
		spRenderPass _renderPass;

		spFramebuffer _framebuffer;
		spTexture _albedo; // RGBA16Unorm
		spTexture _normal; // RGBA16Unorm RGB normal, A is roughness
		spTexture _pos; // RGBA32Sfloat RGB pos, A is metallic
};

typedef std::shared_ptr<GBuffer> spGBuffer;

}

#endif //MANGO_GBUFFER_HPP
