//
// Created by kurono267 on 09.07.18.
//

#include "framebuffer_vk.hpp"
#include "default.hpp"
#include "texture_vk.hpp"
#include "pipeline_vk.hpp"

using namespace mango::vulkan;

void FramebufferVK::create(const int width, const int height, const mango::spRenderPass& renderPass) {
	auto renderPass_vk = std::dynamic_pointer_cast<RenderPassVK>(renderPass);
	create(width,height,renderPass_vk->getVK());
}

void FramebufferVK::create(const int width, const int height, const vk::RenderPass& renderPass){
	std::vector<vk::ImageView> views;
	for(auto v : _attachments){
		auto v_vk = std::dynamic_pointer_cast<TextureViewVK>(v);
		views.push_back(v_vk->getView());
	}
	vk::FramebufferCreateInfo framebufferInfo = vk::FramebufferCreateInfo(
		vk::FramebufferCreateFlags(), // Default
		renderPass, // Current render pass
		views.size(), views.data(), // Attachments
		width, // Width
		height, // Height
		1 // Layers
	);
	auto device_vk = std::dynamic_pointer_cast<DeviceVK>(_device);
	_framebuffer = device_vk->getDevice().createFramebuffer(framebufferInfo);
}

vk::Framebuffer FramebufferVK::getVK(){
	return _framebuffer;
}

FramebufferVK::FramebufferVK(const mango::spDevice &device) : Framebuffer(device) {

}

FramebufferVK::~FramebufferVK() {
	std::cout << "~FramebufferVK" << std::endl;
	if(_framebuffer){
		auto device_vk = std::dynamic_pointer_cast<DeviceVK>(_device);
		device_vk->getDevice().destroyFramebuffer(_framebuffer);
	}
}
