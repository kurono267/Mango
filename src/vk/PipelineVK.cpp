//
// Created by kurono267 on 05.06.18.
//

#include <libshaderc/shaderc.hpp>

#include <unified/Mesh.hpp>
#include <api/Utils.hpp>
#include "PipelineVK.hpp"
#include "ConvertVK.hpp"

using namespace mango::vulkan;

void RenderPassVK::create(const bool isPresent){
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
		if(!a.depth)desc.setInitialLayout(isPresent?vk::ImageLayout::ePresentSrcKHR:vk::ImageLayout::eShaderReadOnlyOptimal);
		else desc.setInitialLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal);
		if(!a.depth)desc.setFinalLayout(isPresent?vk::ImageLayout::ePresentSrcKHR:vk::ImageLayout::eShaderReadOnlyOptimal);
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

	auto deviceVK = Instance::device<DeviceVK>();

	_renderPass = deviceVK->getDevice().createRenderPass(_renderPassInfo);
}

vk::RenderPass RenderPassVK::getVK(){
	return _renderPass;
}

PipelineVK::PipelineVK(const mango::PipelineInfo &rp) : Pipeline(rp) {
	setRenderPass(_renderPattern.getRenderPass());
	for(const auto& shaderPair : _renderPattern.getShaders()){
		addShader(shaderPair.first,shaderPair.second);
	}
	setDescSet(_renderPattern.getDescSets());
	create();
}

PipelineVK::~PipelineVK() {
	std::cout << "~PipelineVK" << std::endl;
}

void PipelineVK::addShader(const mango::ShaderStage &type, const std::string &filename) {
	std::string content = mango::readFile(filename);
	shaderc::Compiler shaderCompiler;

	shaderc_shader_kind shader_kind;
	switch (type){
		case mango::ShaderStage::Vertex: shader_kind = shaderc_vertex_shader;
		break;
		case mango::ShaderStage::Fragment: shader_kind = shaderc_fragment_shader;
		break;
		case mango::ShaderStage::Compute: shader_kind = shaderc_compute_shader;
		break;
		case mango::ShaderStage::Geometry: shader_kind = shaderc_geometry_shader;
		break;
		case mango::ShaderStage::TessellationControl: shader_kind = shaderc_tess_control_shader;
		break;
		case mango::ShaderStage::TessellationEvaluation: shader_kind = shaderc_tess_evaluation_shader;
		break;
		default:
		throw std::logic_error("Shader Stage not supported");
	}

	auto shaderResult = shaderCompiler.CompileGlslToSpv(content,shader_kind,filename.c_str());
	int errors = shaderResult.GetNumErrors();
	if(errors != 0){
		std::cout << shaderResult.GetErrorMessage() << std::endl;
		return;
	}
	std::vector<uint32_t> shaderBinary;
	for(auto b : shaderResult){
		shaderBinary.push_back(b);
	}

	vk::ShaderModuleCreateInfo createInfo(vk::ShaderModuleCreateFlags(),shaderBinary.size()*sizeof(uint32_t),shaderBinary.data());
	auto shaderModule = Instance::device<DeviceVK>()->getDevice().createShaderModule(createInfo);

	_shaderModules.push_back(shaderModule);
	vk::PipelineShaderStageCreateInfo shaderStageInfo(
		vk::PipelineShaderStageCreateFlags(),
		shaderStageVK(type),
		shaderModule,
		"main"
	);
	_shaders.push_back(shaderStageInfo);
}

void PipelineVK::setRenderPass(const spRenderPass &rp) {
	_renderPass = std::dynamic_pointer_cast<RenderPassVK>(rp);
}

void PipelineVK::create() {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

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

	_pLayout = vk_device.createPipelineLayout(_pipelineLayoutInfo);

	// Set Vertex format

	auto sVertexBindDesc = sVertex::bindingDesc();
	auto sVertexAttrDesc = sVertex::attributes();

	auto bindingDescription = vk::VertexInputBindingDescription(sVertexBindDesc.binding,sVertexBindDesc.stride);
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
		_renderPass->getVK(),0);

	_pipeline = vk_device.createGraphicsPipelines(nullptr,pipelineInfo)[0];
}

vk::Pipeline PipelineVK::getVK(){
	return _pipeline;
}

void PipelineVK::setDescSet(const std::vector<mango::spDescSet> &descSets) {
	_descLayouts.clear();
	_descSet.clear();
	for(auto d : descSets){
		auto descSetVK = std::dynamic_pointer_cast<DescSetVK>(d);
		_descSet.push_back(descSetVK);
		_descLayouts.push_back(descSetVK->getLayout());
	}
	_pipelineLayoutInfo = vk::PipelineLayoutCreateInfo(
			vk::PipelineLayoutCreateFlags(),
			static_cast<uint32_t>(_descLayouts.size()), _descLayouts.data());
}

void PipelineVK::setDescSet(const mango::spDescSet &descSet) {
	_descLayouts.resize(1);
	_descLayouts[0] = std::dynamic_pointer_cast<DescSetVK>(descSet)->getLayout();
	_pipelineLayoutInfo = vk::PipelineLayoutCreateInfo(
			vk::PipelineLayoutCreateFlags(),
			static_cast<uint32_t>(_descLayouts.size()), _descLayouts.data());
}

vk::PipelineLayout PipelineVK::getLayout() {
	return _pLayout;
}
