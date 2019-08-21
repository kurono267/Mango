//
// Created by kurono267 on 2019-08-21.
//

#ifndef MANGO_RENDERER_HPP
#define MANGO_RENDERER_HPP

#include <mango.hpp>
#include <scene/Scene.hpp>

namespace mango {

class Renderer {
	public:
		Renderer(const spDevice& device,const glm::ivec2& frameSize);

		void render();

		typedef std::shared_ptr<Renderer> ptr;
		static ptr make(const spDevice& device, const glm::ivec2& frameSize);
	protected:
		spDevice _device;

		Scene _scene;
		glm::ivec2 _frameSize;
		spTexture _frame;

		spDescSet _frameDescSet;
		spPipeline _framePipeline;
		spMesh _quadMesh;
		spRenderPass _renderPass;
		std::vector<spCommandBuffer> _frameCommandBuffers;

		spSemaphore _screenAvailable;
		spSemaphore _renderFinish;
};

}

#endif //MANGO_RENDERER_HPP
