//
// Created by kurono267 on 09.07.18.
//

#include "FramebufferVK.hpp"
#include "default.hpp"
#include "TextureVK.hpp"
#include "PipelineVK.hpp"

using namespace mango::vulkan;

void FramebufferVK::finish(const mango::spRenderPass& renderPass) {
	auto renderPassVK = std::dynamic_pointer_cast<RenderPassVK>(renderPass);

	std::vector<vk::ImageView> views;
	for(auto v : _attachments){
		auto v_vk = std::dynamic_pointer_cast<TextureViewVK>(v);
		views.push_back(v_vk->getView());
	}
	vk::FramebufferCreateInfo framebufferInfo = vk::FramebufferCreateInfo(
			vk::FramebufferCreateFlags(), // Default
			renderPassVK->getVK(), // Current render pass
			views.size(), views.data(), // Attachments
			_size.x, // Width
			_size.y, // Height
			1 // Layers
	);
	_deviceVk = Instance::device<DeviceVK>();
	_framebuffer = _deviceVk.lock()->getDevice().createFramebuffer(framebufferInfo);
}

vk::Framebuffer FramebufferVK::getVK(){
	return _framebuffer;
}

FramebufferVK::FramebufferVK(const glm::ivec2& size) : Framebuffer(size) {

}

FramebufferVK::FramebufferVK(const int width, const int height) :
	Framebuffer(width,height) {

}

FramebufferVK::~FramebufferVK() {
	std::cout << "~FramebufferVK" << std::endl;
	if(_framebuffer){
		_deviceVk.lock()->getDevice().destroyFramebuffer(_framebuffer);
	}
}
