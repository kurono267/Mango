//
// Created by kurono267 on 09.07.18.
//

#ifndef MANGO_FRAMEBUFFERVK_HPP
#define MANGO_FRAMEBUFFERVK_HPP

#include <api/Framebuffer.hpp>
#include "default.hpp"

namespace mango::vulkan {

class FramebufferVK : public Framebuffer {
	public:
		FramebufferVK(const spDevice &device);
		virtual ~FramebufferVK() final;

		void create(const int width, const int height, const spRenderPass& renderPass) final;
		void create(const int width, const int height, const vk::RenderPass& renderPass);

		vk::Framebuffer getVK();
	protected:
		vk::Framebuffer _framebuffer;
};

typedef std::shared_ptr<FramebufferVK> spFramebufferVK;

};

#endif //MANGO_FRAMEBUFFERVK_HPP