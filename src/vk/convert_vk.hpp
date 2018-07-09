//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_CONVERT_VK_HPP
#define MANGO_CONVERT_VK_HPP

#include <mango.hpp>
#include "api/types.hpp"
#include "default.hpp"

namespace mango::vulkan {

vk::Viewport viewportVK(const mango::Viewport& _mango);
vk::Rect2D rect2DVK(const glm::ivec4& _mango);

vk::PolygonMode polygonModeVK(const PolygonMode& _mango);
vk::CullModeFlags cullModeVK(const CullMode& _mango);
vk::FrontFace frontFaceVK(const FrontFace& _mango);
vk::PipelineRasterizationStateCreateInfo rasterizationStateVK(const RasterizationState& _mango);

vk::SampleCountFlagBits sampleCountVK(const SampleCount& _mango);
vk::PipelineMultisampleStateCreateInfo multisampleStateVK(const MultisamplingState& _mango);

vk::CompareOp compareOpVK(const CompareOp& _mango);
vk::PipelineDepthStencilStateCreateInfo depthStateVK(const DepthState& _mango);

vk::BlendFactor blendFactorVK(const BlendFactor& _mango);
vk::BlendOp blendOpVK(const BlendOp& _mango);
vk::ColorComponentFlagBits colorComponenetBitVK(const ColorComponent& _mango);
vk::ColorComponentFlags colorComponentFlagsVK(const std::vector<ColorComponent>& _mango);
vk::PipelineColorBlendStateCreateInfo blendStateVK(const std::vector<BlendAttachmentState>& _mango);

vk::PrimitiveTopology topologyVK(const PrimitiveTopology& _mango);

vk::Format formatVK(const Format& _mango);

vk::ComponentSwizzle componentSwizzleVK(const ComponentSwizzle& _mango);
vk::ComponentMapping componentMappingVK(const ComponentMapping& _mango);

};

#endif //MANGO_CONVERT_VK_HPP
