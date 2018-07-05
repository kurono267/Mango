//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_CONVERT_VK_HPP
#define MANGO_CONVERT_VK_HPP

#include "api/types.hpp"
#include "default.hpp"

namespace mango::vulkan {

vk::Viewport viewportVK(const mango::Viewport& _mango);
vk::Rect2D rect2DVK(const glm::ivec4& _mango);
vk::PolygonMode polygonModeVK(const PolygonMode& _mango);
vk::CullModeFlags cullModeVK(const CullMode& _mango);
vk::FrontFace frontFaceVK(const FrontFace& _mango);
vk::PipelineRasterizationStateCreateInfo rasterizationStateVK(const RasterizationState& _mango);

};

#endif //MANGO_CONVERT_VK_HPP
