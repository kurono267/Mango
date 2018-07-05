//
// Created by kurono267 on 06.07.18.
//

#include "convert_vk.hpp"

using namespace mango::vulkan;

vk::Viewport viewportVK(const mango::Viewport& _mango){
	vk::Viewport _vk;
	_vk.x = _mango.start.x; _vk.y = _mango.start.y;
	_vk.width = _mango.size.x; vk.height = _mango.size.y;
	_vk.minDepth = _mango.minDepth; _vk.maxDepth = _mango.maxDepth;
	return _vk;
}

vk::Rect2D rect2DVK(const glm::ivec4& _mango){
	return vk::Rect2D(vk::Offset2D(_mango.x,_mango.y),vk::Extent2D(_mango.z,_mango.w));
}

vk::PolygonMode polygonModeVK(const mango::PolygonMode& _mango){
	switch (_mango){
		case mango::PolygonMode::Point:
		return vk::PolygonMode::ePoint;
		case mango::PolygonMode::Fill:
		return vk::PolygonMode::eFill;
		case mango::PolygonMode::Line:
		return vk::PolygonMode::eLine;
	}
}

vk::CullModeFlags cullModeVK(const mango::CullMode& _mango){
	switch (_mango){
		case mango::CullMode::Back:
		return vk::CullModeFlagBits::eBack;
		case mango::CullMode::Front:
		return vk::CullModeFlagBits::eFront;
		case mango::CullMode::FrontAndBack:
		return vk::CullModeFlagBits::eFrontAndBack;
		case mango::CullMode::None:
		return vk::CullModeFlagBits::eNone;
	}
}

vk::FrontFace frontFaceVK(const mango::FrontFace& _mango){
	switch(_mango){
		case mango::FrontFace::CCW:
		return vk::FrontFace::eCounterClockwise;
		case mango::FrontFace::CW:
		return vk::FrontFace::eClockwise;
	}
}

vk::PipelineRasterizationStateCreateInfo rasterizationStateVK(const mango::RasterizationState& _mango){
	auto polyMode = mango::vulkan::polygonModeVK(_mango.polyMode);
	auto cullMode = mango::vulkan::cullModeVK(_mango.cullMode);
	auto frontFace = mango::vulkan::frontFaceVK(_mango.face);
	auto _vk = vk::PipelineRasterizationStateCreateInfo(
		vk::PipelineRasterizationStateCreateFlags(),
		(vk::Bool32)_mango.depthClamp,(vk::Bool32)_mango.discard,
		polyMode,cullMode,
		frontFace,
		(vk::Bool32)_mango.depthBias,
		_mango.depthBiasFactor.x,_mango.depthBiasFactor.y,_mango.depthBiasFactor.z,
		_mango.lineWidth);
	return _vk;
}
