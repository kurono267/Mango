//
// Created by kurono267 on 05.06.18.
//

#ifndef MANGO_PIPELINEVK_HPP
#define MANGO_PIPELINEVK_HPP

#include "InstanceVK.hpp"
#include "api/Pipeline.hpp"
#include "DescSetVK.hpp"

#include <iostream>

namespace mango::vulkan {

class RenderPassVK : public RenderPass {
	public:
		RenderPassVK() = default;
		virtual ~RenderPassVK() = default;

		virtual void create(const bool isPresent = false) final;

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
		PipelineVK(const PipelineInfo &rp);
		virtual ~PipelineVK() final;

		static spPipeline make(const PipelineInfo & rp){ return std::make_shared<PipelineVK>(rp); }

		vk::Pipeline getVK();
		vk::PipelineLayout getLayout();
	private:
		void addShader(const ShaderStage& type,const std::string& filename); // Bascily import glsl
		void setRenderPass(const std::shared_ptr<RenderPass>& rp);

		void setDescSet(const std::vector<spDescSet> &descSets);
		void setDescSet(const spDescSet &descSet);

		void create();
	private:
		std::vector<vk::PipelineShaderStageCreateInfo> _shaders;
		vk::PipelineViewportStateCreateInfo      _viewportState;

		std::vector<std::shared_ptr<DescSetVK>> _descSet;
		std::vector<vk::DescriptorSetLayout> _descLayouts;
		vk::PipelineLayoutCreateInfo _pipelineLayoutInfo;

		vk::PushConstantRange _pushConstRange;

		std::shared_ptr<RenderPassVK> _renderPass;
		vk::Pipeline                _pipeline;
		vk::PipelineLayout          _pLayout;

		std::vector<vk::ShaderModule> _shaderModules;
};

};


#endif //MANGO_PIPELINEVK_HPP
