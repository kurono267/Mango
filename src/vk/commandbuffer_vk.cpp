//
// Created by kurono267 on 05.09.18.
//

#include "commandbuffer_vk.hpp"
#include "framebuffer_vk.hpp"
#include "pipeline_vk.hpp"

namespace mango::vulkan {

void CommandBufferVK::setClearColor(const int attachment, const glm::vec4 &color) {
	_clearColors[attachment] = color;
}

void CommandBufferVK::setClearDepthStencil(const int attachment, const float depth, const float stencil) {
	_clearDepthStencil[attachment] = glm::vec2(depth,stencil);
}

void CommandBufferVK::begin() {
	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);
	_cmd.begin(beginInfo);
}

void CommandBufferVK::beginRenderPass(const mango::spRenderPass &renderPass,
                                                     const mango::spFramebuffer &framebuffer,
                                                     const mango::RenderArea &area) {
	// Set Clear color for buffers
	int maxID = 0;
	for(auto c : _clearColors)maxID = std::max(c.first,maxID);
	for(auto d : _clearDepthStencil)maxID = std::max(d.first,maxID);
	std::vector<vk::ClearValue> clearValues;
	if(maxID != 0){
		clearValues.resize(maxID+1);
		for(auto c : _clearColors){
			clearValues[c.first].color = vk::ClearColorValue(std::array<float,4>{c.second.r,c.second.g,c.second.b,c.second.a});
		}
		for(auto d : _clearDepthStencil){
			clearValues[d.first].depthStencil = vk::ClearDepthStencilValue(d.second.x, static_cast<uint32_t>(d.second.y));
		}
	}
	vk::RenderPassBeginInfo renderPassInfo(
		std::dynamic_pointer_cast<RenderPassVK>(renderPass)->getVK(),
		std::dynamic_pointer_cast<FramebufferVK>(framebuffer)->getVK(),
		vk::Rect2D(vk::Offset2D(area.offset.x,area.offset.y),vk::Extent2D(area.size.x,area.size.y)),
		static_cast<uint32_t>(clearValues.size()), clearValues.data()
	);

	_cmd.beginRenderPass(&renderPassInfo,vk::SubpassContents::eInline);
}

void CommandBufferVK::bindPipeline(const mango::spPipeline &pipeline) {
	_cmd.bindPipeline(vk::PipelineBindPoint::eGraphics,std::dynamic_pointer_cast<PipelineVK>(pipeline)->getVK());
}

void CommandBufferVK::bindDescriptorSet(const mango::spPipeline &pipeline) {

}

void CommandBufferVK::endRenderPass() {
	_cmd.endRenderPass();
}

void CommandBufferVK::end() {
	_cmd.end();
}

void CommandBufferVK::create(const spDeviceVK& device){
	vk::CommandBufferAllocateInfo allocInfo(device->getCommandPool(),vk::CommandBufferLevel::ePrimary, 1);
	auto cmds = device->getDevice().allocateCommandBuffers(allocInfo);
	_cmd = cmds[0];
}

vk::CommandBuffer CommandBufferVK::getVK(){
	return _cmd;
}

}