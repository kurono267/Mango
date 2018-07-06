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

		vk::RenderPass create(const spDeviceVK& device);
	protected:
		std::vector<vk::AttachmentDescription>   _attachmentsDesc;
		std::vector<vk::AttachmentReference>     _attachmentsRef;
		vk::AttachmentReference _depthRef;
		vk::AttachmentDescription _depthDesc;

		vk::SubpassDescription    _subPass;
		vk::SubpassDependency     _subPassDep[2];
		vk::RenderPassCreateInfo  _renderPassInfo;
};

class PipelineVK : public mango::Pipeline {
	public:
		PipelineVK(const RenderPattern &rp);
		virtual ~PipelineVK() final;

		void addShader(const ShaderStage& type,const std::string& filename) final; // Bascily import glsl
		void setRenderPass(const std::shared_ptr<RenderPass>& rp) final;

		void create(const spDevice& device) final;

		static spPipeline make(const RenderPattern & rp){ return std::make_shared<PipelineVK>(rp); }
	private:
		vk::Device _device;

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
