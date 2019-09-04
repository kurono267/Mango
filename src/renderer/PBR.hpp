//
// Created by kurono267 on 2019-09-04.
//

#ifndef MANGO_PBR_HPP
#define MANGO_PBR_HPP

#include "GBuffer.hpp"

namespace mango {

class PBR {
	public:
		PBR(const spGBuffer& gbuffer, const glm::ivec2& size);

		void render(const spSemaphore& wait, const spSemaphore& finish);

		spTexture getLightResult();
	protected:
		spTexture _lightResult;
		spGBuffer _gBuffer;

		spPipeline _pipeline;
		spRenderPass _renderPass;
		spFramebuffer _framebuffer;
		spCommandBuffer _commandBuffer;
		spDescSet _descSet;

		spMesh _quad;
};

typedef std::shared_ptr<PBR> spPBR;

}

#endif //MANGO_PBR_HPP
