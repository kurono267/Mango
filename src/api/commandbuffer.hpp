//
// Created by kurono267 on 05.09.18.
//

#ifndef MANGO_COMMANDBUFFER_HPP
#define MANGO_COMMANDBUFFER_HPP

#include <mango.hpp>

namespace mango {

struct RenderArea {
	RenderArea(const glm::ivec2& size_, const glm::ivec2& offset_) : offset(offset_), size(size_)
	{}

	glm::ivec2 offset;
	glm::ivec2 size;
};

class CommandBuffer {
	public:
		CommandBuffer() = default;
		virtual ~CommandBuffer() = default;

		virtual void setClearColor(const int attachment, const glm::vec4& color) = 0;
		virtual void setClearDepthStencil(const int attachment, const float depth = 1.0f,const float stencil = 0.0f) = 0;

		virtual void begin() = 0;
		virtual void beginRenderPass(const spRenderPass& renderPass, const spFramebuffer& framebuffer, const RenderArea& area) = 0;
		virtual void bindPipeline(const spPipeline& pipeline) = 0;
		virtual void bindDescriptorSet(const spPipeline& pipeline = nullptr) = 0;
		virtual void endRenderPass() = 0;
		virtual void end() = 0;
};

typedef std::shared_ptr<CommandBuffer> spCommandBuffer;

};

#endif //MANGO_COMMANDBUFFER_HPP
