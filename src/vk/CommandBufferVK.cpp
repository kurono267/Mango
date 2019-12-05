//
// Created by kurono267 on 05.09.18.
//

#include "CommandBufferVK.hpp"
#include "FramebufferVK.hpp"
#include "PipelineVK.hpp"
#include "BufferVK.hpp"
#include "DescSetVK.hpp"
#include "ComputeVK.hpp"
#include "TextureVK.hpp"

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
			clearValues[c.first].color = vk::ClearColorValue(std::array<float,4>{c.second.x,c.second.y,c.second.z,c.second.w});
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

void CommandBufferVK::bindVertexBuffer(const spBuffer &buffer, uint32_t offset) {
	auto bufferVK = std::dynamic_pointer_cast<BufferVK>(buffer);
	vk::Buffer internal = bufferVK->getVKBuffer();

	vk::DeviceSize vkOffset = offset;
	_cmd.bindVertexBuffers(0,1,&internal,&vkOffset);
}

void CommandBufferVK::bindIndexBuffer(const spBuffer &buffer,uint32_t offset) {
	auto bufferVK = std::dynamic_pointer_cast<BufferVK>(buffer);

	_cmd.bindIndexBuffer(bufferVK->getVKBuffer(),offset,vk::IndexType::eUint32);
}


void CommandBufferVK::drawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset,
							 uint32_t firstInstance) {
	_cmd.drawIndexed(indexCount,instanceCount,firstIndex,vertexOffset,firstInstance);
}

void CommandBufferVK::endRenderPass() {
	_cmd.endRenderPass();
}

void CommandBufferVK::end() {
	_cmd.end();
}

void CommandBufferVK::setViewport(const glm::ivec2& size, const glm::ivec2& offset){
	vk::Viewport viewport(offset.x,offset.y,size.x,size.y);
	_cmd.setViewport(0,1,&viewport);
}

void CommandBufferVK::create(){
	auto device = Instance::device<DeviceVK>();
	vk::CommandBufferAllocateInfo allocInfo(device->getCommandPool(),vk::CommandBufferLevel::ePrimary, 1);
	auto cmds = device->getDevice().allocateCommandBuffers(allocInfo);
	_cmd = cmds[0];
}

CommandBufferVK::~CommandBufferVK() {
	std::cout << "~CommandBufferVK" << std::endl;
	if(_cmd){
		_cmd.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		auto device = Instance::device<DeviceVK>();
		device->getDevice().freeCommandBuffers(device->getCommandPool(),1,&_cmd);
	}
}

vk::CommandBuffer CommandBufferVK::getVK(){
	return _cmd;
}

void CommandBufferVK::bindDescriptorSet(const spPipeline &pipeline, const std::vector<spDescSet> &descSets) {
	auto internalPipeline = std::dynamic_pointer_cast<PipelineVK>(pipeline);
	_descSets.clear();
	for(auto d : descSets){
		_descSets.push_back(std::dynamic_pointer_cast<DescSetVK>(d)->getSet());
	}
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, internalPipeline->getLayout(), 0,
							static_cast<uint32_t>(_descSets.size()), _descSets.data(), 0, nullptr);
}

void CommandBufferVK::bindDescriptorSet(const spPipeline &pipeline, const spDescSet &descSet) {
	auto internalPipeline = std::dynamic_pointer_cast<PipelineVK>(pipeline);
	_descSets.resize(1);
	_descSets[0] = std::dynamic_pointer_cast<DescSetVK>(descSet)->getSet();
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eGraphics, internalPipeline->getLayout(), 0,
							static_cast<uint32_t>(_descSets.size()), _descSets.data(), 0, nullptr);
}

void CommandBufferVK::bindCompute(const spCompute &compute) {
	auto computeVK = std::dynamic_pointer_cast<ComputeVK>(compute);
	_cmd.bindPipeline(vk::PipelineBindPoint::eCompute,computeVK->getPipeline());
}

void CommandBufferVK::dispatch(const size_t &sizeX, const size_t &sizeY, const size_t &sizeZ) {
	_cmd.dispatch(sizeX,sizeY,sizeZ);
}

void CommandBufferVK::bindDescriptorSet(const spCompute &compute, const std::vector<spDescSet> &descSets) {
	auto internalPipeline = std::dynamic_pointer_cast<ComputeVK>(compute);
	_descSets.clear();
	for(auto d : descSets){
		_descSets.push_back(std::dynamic_pointer_cast<DescSetVK>(d)->getSet());
	}
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eCompute, internalPipeline->getLayout(), 0,
							static_cast<uint32_t>(_descSets.size()), _descSets.data(), 0, nullptr);
}

void CommandBufferVK::bindDescriptorSet(const spCompute &compute, const spDescSet &descSet) {
	auto internalPipeline = std::dynamic_pointer_cast<ComputeVK>(compute);
	_descSets.resize(1);
	_descSets[0] = std::dynamic_pointer_cast<DescSetVK>(descSet)->getSet();
	_cmd.bindDescriptorSets(vk::PipelineBindPoint::eCompute, internalPipeline->getLayout(), 0,
							static_cast<uint32_t>(_descSets.size()), _descSets.data(), 0, nullptr);
}

void CommandBufferVK::pushConstants(const spPipeline& pipeline, uint32_t offset, uint32_t size, ShaderStage stage, void* data) {
	auto internalPipeline = std::dynamic_pointer_cast<PipelineVK>(pipeline);
	_cmd.pushConstants(internalPipeline->getLayout(),vulkan::shaderStageVK(stage),offset,size,data);
}

void CommandBufferVK::clearTexture(const spTexture &texture, const glm::vec4 &color) {
	auto internalTexture = std::dynamic_pointer_cast<TextureVK>(texture);
	vk::ImageSubresourceRange imageSubresourceRange(vk::ImageAspectFlagBits::eColor,0,texture->mipLevels(),0,1);
	_cmd.clearColorImage(internalTexture->getImage(),vk::ImageLayout::eGeneral,vk::ClearColorValue(std::array<float,4>{color.x,color.y,color.z,color.w}),imageSubresourceRange);
}

}