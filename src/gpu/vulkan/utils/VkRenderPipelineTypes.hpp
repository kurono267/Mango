//
// Created by kurono267 on 20.07.2021.
//

#pragma once

#include "../VulkanResources.hpp"
#include "../../general/RenderPipelineTypes.hpp"

namespace mango {

vk::Viewport viewportVK(const Viewport& _mango);
vk::Rect2D rect2DVK(const glm::ivec4& _mango);

vk::PolygonMode polygonModeVK(const PolygonMode& _mango);
vk::CullModeFlags cullModeVK(const CullMode& _mango);
vk::FrontFace frontFaceVK(const FrontFace& _mango);
vk::PipelineRasterizationStateCreateInfo rasterizationStateVK(const RasterizationState& _mango);

//vk::SampleCountFlagBits sampleCountVK(const SampleCount& _mango);
//vk::PipelineMultisampleStateCreateInfo multisampleStateVK(const MultisamplingState& _mango);

vk::CompareOp compareOpVK(const CompareOp& _mango);
vk::PipelineDepthStencilStateCreateInfo depthStateVK(const DepthState& _mango);

vk::BlendFactor blendFactorVK(const BlendFactor& _mango);
vk::BlendOp blendOpVK(const BlendOp& _mango);
vk::ColorComponentFlags colorComponentFlagsVK(const uint16_t& _mango);
vk::PipelineColorBlendAttachmentState blendStateVK(const BlendState& _mango);

vk::PrimitiveTopology topologyVK(const PrimitiveTopology& _mango);

vk::ShaderStageFlagBits shaderStageVK(const ShaderStage &_mango);

}


