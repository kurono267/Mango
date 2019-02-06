//
// Created by kurono267 on 05.06.18.
//

#ifndef MANGO_PIPELINE_VK_HPP
#define MANGO_PIPELINE_VK_HPP

#include "instance_vk.hpp"
#include "api/pipeline.hpp"

#include <iostream>

namespace mango::vulkan {

class RenderPassVK : public RenderPass {
	public:
		RenderPassVK() = default;
		virtual ~RenderPassVK() = default;

		void create(const spDeviceVK& device);

		vk::RenderPass getVK();
	protected:
		std::vector<vk::AttachmentDescription>   _attachmentsDesc;
		std::vector<vk::AttachmentReference>     _attachmentsRef;
		vk::AttachmentReference _depthRef;
		vk::AttachmentDescription _depthDesc;

		vk::SubpassDescription    _subPass;
		vk::SubpassDependency     _subPassDep[2];
		vk::RenderPassCreateInfo  _renderPassInfo;

		vk::RenderPass _renderPass;
};

typedef std::shared_ptr<RenderPassVK> spRenderPassVK;

class PipelineVK : public mango::Pipeline {
	public:
		PipelineVK(const spDevice& device,const PipelineInfo &rp);
		virtual ~PipelineVK() final;

		static spPipeline make(const spDevice& device,const PipelineInfo & rp){ return std::make_shared<PipelineVK>(device,rp); }

		vk::Pipeline getVK();
		vk::PipelineLayout getLayout();
	private:
		void addShader(const ShaderStage& type,const std::string& filename); // Bascily import glsl
		void setRenderPass(const std::shared_ptr<RenderPass>& rp);

		void setDescSet(const std::vector<spDescSet> &descSets);
		void setDescSet(const spDescSet &descSet);

		void create();
	private:
		spDeviceVK _device;
		vk::Device _vk_device;

		std::vector<vk::PipelineShaderStageCreateInfo> _shaders;
		vk::PipelineViewportStateCreateInfo      _viewportState;

		std::vector<vk::DescriptorSetLayout> _descLayouts;
		vk::PipelineLayoutCreateInfo _pipelineLayoutInfo;

		vk::PushConstantRange _pushConstRange;

		std::shared_ptr<RenderPassVK> _renderPass;
		vk::Pipeline                _pipeline;
		vk::PipelineLayout          _pLayout;

		std::vector<vk::ShaderModule> _shaderModules;
};

};


#endif //MANGO_PIPELINE_VK_HPP
