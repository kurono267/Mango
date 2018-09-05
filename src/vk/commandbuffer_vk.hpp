//
// Created by kurono267 on 05.09.18.
//

#ifndef MANGO_COMMANDBUFFER_VK_HPP
#define MANGO_COMMANDBUFFER_VK_HPP

#include "../api/commandbuffer.hpp"
#include "default.hpp"

namespace mango::vulkan {

class DeviceVK;
typedef std::shared_ptr<DeviceVK> spDeviceVK;

class CommandBufferVK : public CommandBuffer {
	public:
		void setClearColor(const int attachment, const glm::vec4 &color) final;
		void setClearDepthStencil(const int attachment, const float depth, const float stencil) final;

		void begin() final;
		void beginRenderPass(const spRenderPass &renderPass, const spFramebuffer &framebuffer, const RenderArea &area) final;
		void bindPipeline(const spPipeline &pipeline) final;
		void bindDescriptorSet(const spPipeline &pipeline) final;
		void endRenderPass() final;
		void end() final;

		void create(const spDeviceVK device);
	protected:
		vk::CommandBuffer _cmd;
		std::unordered_map<int,glm::vec4> _clearColors;
		std::unordered_map<int,glm::vec2> _clearDepthStencil;
};

};

#endif //MANGO_COMMANDBUFFER_VK_HPP
