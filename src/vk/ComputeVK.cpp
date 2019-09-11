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
}

vk::Pipeline ComputeVK::getPipeline() {
	return _pipeline;
}

vk::PipelineLayout ComputeVK::getLayout() {
	return _pipelineLayout;
}
