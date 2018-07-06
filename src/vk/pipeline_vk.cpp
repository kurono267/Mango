//
// Created by kurono267 on 05.06.18.
//

#include <api/mesh.hpp>
#include "pipeline_vk.hpp"
#include "convert_vk.hpp"

using namespace mango::vulkan;

vk::RenderPass RenderPassVK::create(const spDeviceVK& device){
	_attachmentsDesc.clear();
	_attachmentsRef.clear();
	for(auto a : _attachments){
		vk::AttachmentDescription desc;

		desc.setFormat(mango::vulkan::formatVK(a.format));
		desc.setSamples(mango::vulkan::sampleCountVK(a.samples));
		desc.setLoadOp(vk::AttachmentLoadOp::eClear);
		if(!a.depth)desc.setStoreOp(vk::AttachmentStoreOp::eStore);
		else desc.setStoreOp(vk::AttachmentStoreOp::eDontCare);
		desc.setStencilLoadOp(vk::AttachmentLoadOp::eDontCare);
		desc.setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
		if(!a.depth)desc.setInitialLayout(vk::ImageLayout::ePresentSrcKHR);
		else desc.setInitialLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
		if(!a.depth)desc.setFinalLayout(vk::ImageLayout::ePresentSrcKHR);
		else desc.setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		vk::AttachmentReference ref;
		ref.setAttachment(a.index);
		if(!a.depth)ref.setLayout(vk::ImageLayout::eColorAttachmentOptimal);
		else ref.setLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		if(a.depth){
			_depthDesc = desc;
			_depthRef = ref;
		} else {
			_attachmentsDesc.push_back(desc);
			_attachmentsRef.push_back(ref);
		}
	}
	_attachmentsDesc.push_back(_depthDesc);
	_attachmentsRef.push_back(_depthRef);

	_subPass.setPipelineBindPoint(vk::PipelineBindPoint::eGraphics);
	_subPass.setColorAttachmentCount(_attachmentsRef.size()-1);
	_subPass.setPColorAttachments(_attachmentsRef.data());
	_subPass.setPDepthStencilAttachment(&_depthRef);

	_subPassDep[0].srcSubpass = VK_SUBPASS_EXTERNAL;
	_subPassDep[0].dstSubpass = 0;
	_subPassDep[0].srcStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	_subPassDep[0].dstStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	_subPassDep[0].srcAccessMask = vk::AccessFlagBits::eMemoryRead;
	_subPassDep[0].dstAccessMask = vk::AccessFlagBits::eColorAttachmentRead| vk::AccessFlagBits::eColorAttachmentWrite;
	_subPassDep[0].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	_subPassDep[1].srcSubpass = 0;
	_subPassDep[1].dstSubpass = VK_SUBPASS_EXTERNAL;
	_subPassDep[1].srcStageMask = vk::PipelineStageFlagBits::eColorAttachmentOutput;
	_subPassDep[1].dstStageMask = vk::PipelineStageFlagBits::eBottomOfPipe;
	_subPassDep[1].srcAccessMask = vk::AccessFlagBits::eColorAttachmentRead| vk::AccessFlagBits::eColorAttachmentWrite;
	_subPassDep[1].dstAccessMask = vk::AccessFlagBits::eMemoryRead;
	_subPassDep[1].dependencyFlags = vk::DependencyFlagBits::eByRegion;

	_renderPassInfo.setAttachmentCount(_attachmentsDesc.size());
	_renderPassInfo.setPAttachments(_attachmentsDesc.data());
	_renderPassInfo.setSubpassCount(1);
	_renderPassInfo.setPSubpasses(&_subPass);
	_renderPassInfo.setDependencyCount(2);
	_renderPassInfo.setPDependencies(_subPassDep);

	return device->getDevice().createRenderPass(_renderPassInfo);
}

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

	auto sVertexBindDesc = sVertex::bindingDesc();
	auto sVertexAttrDesc = sVertex::attributes();

	auto bindingDescription = vk::VertexInputBindingDescription(sVertexBindDesc.binding,sVertexBindDesc.binding);
    std::vector<vk::VertexInputAttributeDescription> attributeDescriptions;
    for(auto attr : sVertexAttrDesc){
        attributeDescriptions.push_back(vk::VertexInputAttributeDescription(
            attr.location,attr.binding,formatVK(attr.format),attr.offset
        ));
    }

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
	auto vk_multisampling = multisampleStateVK(_renderPattern.getMultisamplingState());

	// Convert Depth Stencil state
	auto vk_depthstencil = depthStateVK(_renderPattern.getDepthState());

	// Convert Blend State
	auto vk_blendstate = blendStateVK(_renderPattern.getBlendAttachments());

	auto vk_assembly = vk::PipelineInputAssemblyStateCreateInfo(vk::PipelineInputAssemblyStateCreateFlags(),topologyVK(_renderPattern.getTopology()));

	vk::GraphicsPipelineCreateInfo pipelineInfo(vk::PipelineCreateFlags(),
		_shaders.size(),_shaders.data(),
		&vertexInputInfo,&vk_assembly,
		nullptr,
		&_viewportState,
		&vk_rasterizer,
		&vk_multisampling,
		&vk_depthstencil, // Depth and stencil
		&vk_blendstate,
		dynamicStates.size()==0?nullptr:&dynamicStatesCreteInfo,
		_pLayout,
		_renderPass->create(vk_device),0);

	_pipeline = _device.createGraphicsPipelines(nullptr,pipelineInfo)[0];
}


