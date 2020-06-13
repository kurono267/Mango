//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_FRAMEBUFFERVK_HPP
#define MANGO_FRAMEBUFFERVK_HPP

#include "../api/Framebuffer.hpp"
#include "DeviceVK.hpp"
#include "default.hpp"

namespace mango::vulkan {

class FramebufferVK : public Framebuffer {
	public:
		FramebufferVK(const glm::ivec2& size);
		FramebufferVK(const int width, const int height);
		virtual ~FramebufferVK() final;

		void finish(const spRenderPass& renderPass) final;

		vk::Framebuffer getVK();
	protected:
		vk::Framebuffer _framebuffer;
		std::weak_ptr<DeviceVK> _deviceVk;
};

typedef std::shared_ptr<FramebufferVK> spFramebufferVK;

};

#endif //MANGO_FRAMEBUFFERVK_HPP
