//
// Created by kurono267 on 05.09.18.
//

#ifndef MANGO_COMMANDBUFFERVK_HPP
#define MANGO_COMMANDBUFFERVK_HPP

#include "../api/CommandBuffer.hpp"
#include "default.hpp"
#include <unordered_map>

namespace mango::vulkan {

class DeviceVK;
typedef std::shared_ptr<DeviceVK> spDeviceVK;
class BufferVK;
typedef std::shared_ptr<BufferVK> spBufferVK;

class CommandBufferVK : public CommandBuffer {
	public:
		~CommandBufferVK() override;

		void setClearColor(int attachment, const glm::vec4 &color) final;
		void setClearDepthStencil(int attachment, float depth, float stencil) final;

		void clearTexture(const spTexture& texture,const glm::vec4& color) final;

		void begin() final;
		void beginRenderPass(const spRenderPass &renderPass, const spFramebuffer &framebuffer, const RenderArea &area) final;
		void bindPipeline(const spPipeline &pipeline) final;
		void bindCompute(const spCompute& compute) final;
		void dispatch(const size_t& sizeX, const size_t& sizeY, const size_t& sizeZ) final;

		void bindDescriptorSet(const spPipeline &pipeline, const std::vector<spDescSet> &descSets) override;
		void bindDescriptorSet(const spPipeline &pipeline, const spDescSet &descSet) override;

		void bindDescriptorSet(const spCompute &compute, const std::vector<spDescSet> &descSets) final;
		void bindDescriptorSet(const spCompute &compute, const spDescSet &descSet) final;

		void pushConstants(const spPipeline& pipeline, uint32_t offset, uint32_t size, ShaderStage stage, void* data) final;

		void bindVertexBuffer(const spBuffer& buffer, uint32_t offset = 0) final;
		void bindIndexBuffer(const spBuffer& buffer,uint32_t offset = 0) final;
		void bindIndexBuffer16(const spBuffer& buffer,uint32_t offset = 0) final;
		void drawIndexed(uint32_t indexCount, uint32_t instanceCount = 1, uint32_t firstIndex = 0, int32_t vertexOffset = 0, uint32_t firstInstance = 0) final;
		void endRenderPass() final;
		void end() final;

		void setViewport(const glm::ivec2& size, const glm::ivec2& offset = glm::ivec2(0)) final;
		void setScissor(const glm::ivec2& size, const glm::ivec2& offset = glm::ivec2(0)) final;

		void reset() final;

		void create();

		vk::CommandBuffer getVK();
	protected:
		vk::CommandBuffer _cmd;
		std::unordered_map<int,glm::vec4> _clearColors;
		std::unordered_map<int,glm::vec2> _clearDepthStencil;
		std::vector<vk::DescriptorSet> _descSets;
};

};

#endif //MANGO_COMMANDBUFFERVK_HPP
