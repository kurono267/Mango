//
// Created by kurono267 on 2019-09-05.
//

#include "ComputeVK.hpp"
#include "InstanceVK.hpp"
#include "DescSetVK.hpp"
#include "ShaderVK.hpp"

using namespace mango;
using namespace mango::vulkan;

ComputeVK::ComputeVK(const std::string& filename, const std::vector<spDescSet>& descSets) : Compute(filename,descSets) {
	init();
}

void ComputeVK::run(const spSemaphore& waitForIt,const spSemaphore& result,const int sizeX, const int sizeY, const int sizeZ) {
	if(sizeX != _size.x || sizeY != _size.y || sizeZ != _size.z){ // Recreate command buffer when size changed
		if(_commandBuffer){
			_commandBuffer.reset(vk::CommandBufferResetFlagBits::eReleaseResources);
		}
		_size = glm::ivec3(sizeX,sizeY,sizeZ);
		initCommandBuffer();
	}

	auto waitVK = std::dynamic_pointer_cast<SemaphoreVK>(waitForIt);
	auto resultVK = std::dynamic_pointer_cast<SemaphoreVK>(result);

	vk::Semaphore waitSemaphores[] = {waitVK->getVK()};
	vk::Semaphore signalSemaphores[] = {resultVK->getVK()};
	vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eComputeShader|vk::PipelineStageFlagBits::eColorAttachmentOutput};

	auto device = Instance::device<DeviceVK>();

	vk::SubmitInfo submitInfo(
			waitForIt?1:0, waitForIt?waitSemaphores: nullptr,
			waitStages,
			1, &_commandBuffer,
			1, signalSemaphores
	);

	device->getComputeQueue().submit(submitInfo,nullptr);
}

void ComputeVK::initCommandBuffer(){
	auto device = Instance::device<DeviceVK>();
	auto vk_device = device->getDevice();
	auto commandPool = device->getComputeCommandPool();

	if(!_commandBuffer) {
		vk::CommandBufferAllocateInfo allocInfo(commandPool, vk::CommandBufferLevel::ePrimary, 1);
		_commandBuffer = vk_device.allocateCommandBuffers(allocInfo)[0];
	}

	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eSimultaneousUse);

	_commandBuffer.begin(&beginInfo);

	std::vector<vk::DescriptorSet> descSets;
	for(int i = 0;i<_descSets.size();++i){
		auto d = std::dynamic_pointer_cast<DescSetVK>(_descSets[i]);
		descSets.push_back(d->getSet());
	}

	_commandBuffer.bindPipeline(vk::PipelineBindPoint::eCompute, _pipeline);
	_commandBuffer.bindDescriptorSets(vk::PipelineBindPoint::eCompute, _pipelineLayout, 0, descSets.size(), descSets.data(), 0, nullptr);
	_commandBuffer.dispatch(_size.x,_size.y,_size.z);

	_commandBuffer.end();
}

void ComputeVK::init() {
	std::cout << "ComputeVK::init" << std::endl;
	auto device = Instance::device<DeviceVK>();
	auto vk_device = device->getDevice();

	std::vector<vk::DescriptorSetLayout> descLayouts;
	for(int i = 0;i<_descSets.size();++i){
		auto d = std::dynamic_pointer_cast<DescSetVK>(_descSets[i]);
		descLayouts.push_back(d->getLayout());
	}

	auto layoutInfo = vk::PipelineLayoutCreateInfo(
			vk::PipelineLayoutCreateFlags(),
			descLayouts.size(), descLayouts.data());

	_pipelineLayout = vk_device.createPipelineLayout(layoutInfo);

	auto shaderModule = ShaderVK::createShader(ShaderStage::Compute,_filename);
	if(!shaderModule)
		return;

	vk::PipelineShaderStageCreateInfo shaderStageInfo(
			vk::PipelineShaderStageCreateFlags(),
			vk::ShaderStageFlagBits::eCompute,
			*shaderModule,
			"main"
	);

	vk::ComputePipelineCreateInfo info(
			vk::PipelineCreateFlags(),
			shaderStageInfo,
			_pipelineLayout
	);

	_pipeline = vk_device.createComputePipeline(nullptr,info);
	_commandBuffer = nullptr;
}
