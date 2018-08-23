//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_FRAMEBUFFER_VK_HPP
#define MANGO_FRAMEBUFFER_VK_HPP

#include <api/framebuffer.hpp>
#include "default.hpp"

namespace mango::vulkan {

class FramebufferVK : public Framebuffer {
	public:
		FramebufferVK(const spDevice &device);

		void create(const int width, const int height, const spPipeline& pipeline) final;
		void create(const int width, const int height, const vk::RenderPass& renderPass);
	protected:
		vk::Framebuffer _framebuffer;
};

};

#endif //MANGO_FRAMEBUFFER_VK_HPP
