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

		virtual void setClearColor(int attachment, const glm::vec4& color) = 0;
		virtual void setClearDepthStencil(int attachment, float depth = 1.0f,float stencil = 0.0f) = 0;

		virtual void clearTexture(const spTexture& texture,const glm::vec4& color) = 0;

		virtual void begin() = 0;
		virtual void beginRenderPass(const spRenderPass& renderPass, const spFramebuffer& framebuffer, const RenderArea& area) = 0;
		virtual void bindPipeline(const spPipeline& pipeline) = 0;
		virtual void bindCompute(const spCompute& compute) = 0;
		virtual void dispatch(const size_t& sizeX, const size_t& sizeY, const size_t& sizeZ) = 0;
		virtual void bindDescriptorSet(const spPipeline& pipeline, const std::vector<spDescSet>& descSets) = 0;
		virtual void bindDescriptorSet(const spPipeline& pipeline, const spDescSet& descSet) = 0;
		virtual void bindDescriptorSet(const spCompute &compute, const std::vector<spDescSet> &descSets) = 0;
		virtual void bindDescriptorSet(const spCompute &compute, const spDescSet &descSet) = 0 ;
		virtual void pushConstants(const spPipeline& pipeline, uint32_t offset, uint32_t size, ShaderStage stage, void* data) = 0;
		virtual void bindVertexBuffer(const spBuffer& buffer, uint32_t offset = 0) = 0;
		virtual void bindIndexBuffer(const spBuffer& buffer,uint32_t offset = 0) = 0;
		virtual void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0) = 0;
		virtual void endRenderPass() = 0;
		virtual void end() = 0;

		virtual void setViewport(const glm::ivec2& size, const glm::ivec2& offset = glm::ivec2(0)) = 0;
};

typedef std::shared_ptr<CommandBuffer> spCommandBuffer;

};

#endif //MANGO_COMMANDBUFFER_HPP
