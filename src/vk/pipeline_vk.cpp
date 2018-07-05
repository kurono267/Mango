//
// Created by kurono267 on 05.06.18.
//

#include "pipeline_vk.hpp"
#include "convert_vk.hpp"

using namespace mango::vulkan;

PipelineVK::PipelineVK(const mango::RenderPattern &rp) : Pipeline(rp) {}

PipelineVK::~PipelineVK() {
	std::cout << "~PipelineVK" << std::endl;
}

void PipelineVK::addShader(const mango::ShaderStage &type, const std::string &filename) {

}

void PipelineVK::setRenderPass(const spRenderPass &rp) {
	_renderPass = std::dynamic_pointer_cast<RenderPassVK>(rp);
}

void PipelineVK::create(const mango::spDevice &device) {
	std::shared_ptr<DeviceVK> vk_device = std::dynamic_pointer_cast<DeviceVK>(device);
	_device = vk_device->getDevice();

	std::vector<vk::DynamicState> dynamicStates;
	if(_renderPattern.isDynamicScissor()){
		dynamicStates.push_back(vk::DynamicState::eScissor);
	}
	if(_renderPattern.isDynamicViewport()){
		dynamicStates.push_back(vk::DynamicState::eViewport);
	}
	vk::PipelineDynamicStateCreateInfo dynamicStatesCreteInfo;
	dynamicStatesCreteInfo.pDynamicStates    = dynamicStates.data();
	dynamicStatesCreteInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());

	_pLayout = _device.createPipelineLayout(_pipelineLayoutInfo);

	// Set Vertex format

	auto bindingDescription = vertexBinding;
    auto attributeDescriptions = vertexAttrib;

    vk::PipelineVertexInputStateCreateInfo vertexInputInfo(
    	vk::PipelineVertexInputStateCreateFlags(),
    	1,&bindingDescription,
    	attributeDescriptions.size(),attributeDescriptions.data()
    );

	// Convert Viewport and Scissor
	auto vk_viewport = viewportVK(_renderPattern.getViewport());
	auto vk_scissor = rect2DVK(_renderPattern.getScissor());
	_viewportState = vk::PipelineViewportStateCreateInfo(vk::PipelineViewportStateCreateFlags(),1,&vk_viewport,1,&vk_scissor);

	// Convert Rasterizer state
	auto vk_rasterizer = rasterizationStateVK(_renderPattern.getRasterizationState());

	// Convert MultiSampling state


	vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
		_shaders.size(),_shaders.data(),
		&vertexInputInfo,&_renderpattern._assembly,
		nullptr,
		&_viewportState,
		&vk_rasterizer,
		&_renderpattern._multisampling,
		&_renderpattern._depthStencil, // Depth and stencil
		&_renderpattern._blend,
		dynamicStates.size()==0?nullptr:&dynamicStatesCreteInfo,
		_pLayout,
		_renderPass,0);

	_pipeline = _device.createGraphicsPipelines(nullptr,pipelineInfo)[0];
}


