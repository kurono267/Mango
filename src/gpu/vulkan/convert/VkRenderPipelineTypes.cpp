//
// Created by kurono267 on 20.07.2021.
//

#include "VkRenderPipelineTypes.hpp"

namespace mango {

vk::Viewport viewportVK(const Viewport &_mango) {
	vk::Viewport _vk;
	_vk.x = _mango.start.x;
	_vk.y = _mango.size.y - _mango.start.y;
	_vk.width = _mango.size.x;
	_vk.height = -_mango.size.y;
	_vk.minDepth = _mango.minDepth;
	_vk.maxDepth = _mango.maxDepth;
	return _vk;
}

vk::Rect2D rect2DVK(const glm::ivec4 &_mango) {
	return vk::Rect2D(vk::Offset2D(_mango.x, _mango.y), vk::Extent2D(_mango.z, _mango.w));
}

vk::PolygonMode polygonModeVK(const PolygonMode &_mango) {
	switch (_mango) {
		case PolygonMode::Point:
			return vk::PolygonMode::ePoint;
		case PolygonMode::Fill:
			return vk::PolygonMode::eFill;
		case PolygonMode::Line:
			return vk::PolygonMode::eLine;
	}
}

vk::CullModeFlags cullModeVK(const CullMode &_mango) {
	switch (_mango) {
		case CullMode::Back:
			return vk::CullModeFlagBits::eBack;
		case CullMode::Front:
			return vk::CullModeFlagBits::eFront;
		case CullMode::FrontAndBack:
			return vk::CullModeFlagBits::eFrontAndBack;
		case CullMode::None:
			return vk::CullModeFlagBits::eNone;
	}
}

vk::FrontFace frontFaceVK(const FrontFace &_mango) {
	switch (_mango) {
		case FrontFace::CCW:
			return vk::FrontFace::eCounterClockwise;
		case FrontFace::CW:
			return vk::FrontFace::eClockwise;
	}
}

vk::PipelineRasterizationStateCreateInfo rasterizationStateVK(const RasterizationState &_mango) {
	auto polyMode = polygonModeVK(_mango.polyMode);
	auto cullMode = cullModeVK(_mango.cullMode);
	auto frontFace = frontFaceVK(_mango.face);
	auto _vk = vk::PipelineRasterizationStateCreateInfo(
			vk::PipelineRasterizationStateCreateFlags(),
			(vk::Bool32) _mango.depthClamp, (vk::Bool32) _mango.discard,
			polyMode, cullMode,
			frontFace,
			(vk::Bool32) _mango.depthBias,
			_mango.depthBiasFactor.x, _mango.depthBiasFactor.y, _mango.depthBiasFactor.z,
			_mango.lineWidth);
	return _vk;
}
/*
vk::SampleCountFlagBits sampleCountVK(const SampleCount &_mango) {
	switch (_mango) {
		case SampleCount::s1:
			return vk::SampleCountFlagBits::e1;
		case SampleCount::s2:
			return vk::SampleCountFlagBits::e2;
		case SampleCount::s4:
			return vk::SampleCountFlagBits::e4;
		case SampleCount::s8:
			return vk::SampleCountFlagBits::e8;
		case SampleCount::s16:
			return vk::SampleCountFlagBits::e16;
		case SampleCount::s32:
			return vk::SampleCountFlagBits::e32;
		case SampleCount::s64:
			return vk::SampleCountFlagBits::e64;
	}
}

vk::PipelineMultisampleStateCreateInfo multisampleStateVK(const MultisamplingState &_mango) {
	auto _vk = vk::PipelineMultisampleStateCreateInfo(
			vk::PipelineMultisampleStateCreateFlags(),
			sampleCountVK(_mango.samples),
			static_cast<vk::Bool32>(_mango.sampleShading),
			_mango.minSampleShading,
			nullptr,
			static_cast<vk::Bool32>(_mango.alphaToCoverageEnable),
			static_cast<vk::Bool32>(_mango.alphaToOneEnable));
	return _vk;
}*/

vk::CompareOp compareOpVK(const CompareOp &_mango) {
	switch (_mango) {
		case CompareOp::Always:
			return vk::CompareOp::eAlways;
		case CompareOp::Greater:
			return vk::CompareOp::eGreater;
		case CompareOp::GreaterOrEqual:
			return vk::CompareOp::eGreaterOrEqual;
		case CompareOp::Less:
			return vk::CompareOp::eLess;
		case CompareOp::Never:
			return vk::CompareOp::eNever;
		case CompareOp::LessOrEqual:
			return vk::CompareOp::eLessOrEqual;
		case CompareOp::NotEqual:
			return vk::CompareOp::eNotEqual;
		case CompareOp::Equal:
			return vk::CompareOp::eEqual;
	}
}

vk::PipelineDepthStencilStateCreateInfo depthStateVK(const DepthState &_mango) {
	auto _vk = vk::PipelineDepthStencilStateCreateInfo(
			vk::PipelineDepthStencilStateCreateFlags(),
			static_cast<vk::Bool32>(_mango.enable),
			static_cast<vk::Bool32>(_mango.write),
			compareOpVK(_mango.comp)
	);
	return _vk;
}

vk::BlendFactor blendFactorVK(const BlendFactor &_mango) {
	switch (_mango) {
		case BlendFactor::Zero:
			return vk::BlendFactor::eZero;
		case BlendFactor::One:
			return vk::BlendFactor::eOne;
		case BlendFactor::SrcColor:
			return vk::BlendFactor::eSrcColor;
		case BlendFactor::OneMinusSrcColor:
			return vk::BlendFactor::eOneMinusSrcColor;
		case BlendFactor::DstColor:
			return vk::BlendFactor::eDstColor;
		case BlendFactor::OneMinusDstColor:
			return vk::BlendFactor::eOneMinusDstColor;
		case BlendFactor::SrcAlpha:
			return vk::BlendFactor::eSrcAlpha;
		case BlendFactor::OneMinusSrcAlpha:
			return vk::BlendFactor::eOneMinusSrcAlpha;
		case BlendFactor::DstAlpha:
			return vk::BlendFactor::eDstAlpha;
		case BlendFactor::OneMinusDstAlpha:
			return vk::BlendFactor::eOneMinusDstAlpha;
		case BlendFactor::ConstantColor:
			return vk::BlendFactor::eConstantColor;
		case BlendFactor::OneMinusConstantColor:
			return vk::BlendFactor::eOneMinusConstantColor;
		case BlendFactor::ConstantAlpha:
			return vk::BlendFactor::eConstantAlpha;
		case BlendFactor::OneMinusConstantAlpha:
			return vk::BlendFactor::eOneMinusConstantAlpha;
		case BlendFactor::SrcAlphaSaturate:
			return vk::BlendFactor::eSrcAlphaSaturate;
		case BlendFactor::Src1Color:
			return vk::BlendFactor::eSrc1Color;
		case BlendFactor::OneMinusSrc1Color:
			return vk::BlendFactor::eOneMinusSrc1Color;
		case BlendFactor::Src1Alpha:
			return vk::BlendFactor::eSrc1Alpha;
		case BlendFactor::OneMinusSrc1Alpha:
			return vk::BlendFactor::eOneMinusSrc1Alpha;
	}
}

vk::BlendOp blendOpVK(const BlendOp &_mango) {
	switch (_mango) {
		case BlendOp::Add:
			return vk::BlendOp::eAdd;
		case BlendOp::Subtract:
			return vk::BlendOp::eSubtract;
		case BlendOp::ReverseSubtract:
			return vk::BlendOp::eReverseSubtract;
		case BlendOp::Min:
			return vk::BlendOp::eMin;
		case BlendOp::Max:
			return vk::BlendOp::eMax;
		case BlendOp::ZeroEXT:
			return vk::BlendOp::eZeroEXT;
		case BlendOp::SrcEXT:
			return vk::BlendOp::eSrcEXT;
		case BlendOp::DstEXT:
			return vk::BlendOp::eDstEXT;
		case BlendOp::SrcOverEXT:
			return vk::BlendOp::eSrcOverEXT;
		case BlendOp::DstOverEXT:
			return vk::BlendOp::eDstOverEXT;
		case BlendOp::SrcInEXT:
			return vk::BlendOp::eSrcInEXT;
		case BlendOp::DstInEXT:
			return vk::BlendOp::eDstInEXT;
		case BlendOp::SrcOutEXT:
			return vk::BlendOp::eSrcOutEXT;
		case BlendOp::DstOutEXT:
			return vk::BlendOp::eDstOutEXT;
		case BlendOp::SrcAtopEXT:
			return vk::BlendOp::eSrcAtopEXT;
		case BlendOp::DstAtopEXT:
			return vk::BlendOp::eDstAtopEXT;
		case BlendOp::XorEXT:
			return vk::BlendOp::eXorEXT;
		case BlendOp::MultiplyEXT:
			return vk::BlendOp::eMultiplyEXT;
		case BlendOp::ScreenEXT:
			return vk::BlendOp::eScreenEXT;
		case BlendOp::OverlayEXT:
			return vk::BlendOp::eOverlayEXT;
		case BlendOp::DarkenEXT:
			return vk::BlendOp::eDarkenEXT;
		case BlendOp::LightenEXT:
			return vk::BlendOp::eLightenEXT;
		case BlendOp::ColordodgeEXT:
			return vk::BlendOp::eColordodgeEXT;
		case BlendOp::ColorburnEXT:
			return vk::BlendOp::eColorburnEXT;
		case BlendOp::HardlightEXT:
			return vk::BlendOp::eHardlightEXT;
		case BlendOp::SoftlightEXT:
			return vk::BlendOp::eSoftlightEXT;
		case BlendOp::DifferenceEXT:
			return vk::BlendOp::eDifferenceEXT;
		case BlendOp::ExclusionEXT:
			return vk::BlendOp::eExclusionEXT;
		case BlendOp::InvertEXT:
			return vk::BlendOp::eInvertEXT;
		case BlendOp::InvertRgbEXT:
			return vk::BlendOp::eInvertRgbEXT;
		case BlendOp::LineardodgeEXT:
			return vk::BlendOp::eLineardodgeEXT;
		case BlendOp::LinearburnEXT:
			return vk::BlendOp::eLinearburnEXT;
		case BlendOp::VividlightEXT:
			return vk::BlendOp::eVividlightEXT;
		case BlendOp::LinearlightEXT:
			return vk::BlendOp::eLinearlightEXT;
		case BlendOp::PinlightEXT:
			return vk::BlendOp::ePinlightEXT;
		case BlendOp::HardmixEXT:
			return vk::BlendOp::eHardmixEXT;
		case BlendOp::HslHueEXT:
			return vk::BlendOp::eHslHueEXT;
		case BlendOp::HslSaturationEXT:
			return vk::BlendOp::eHslSaturationEXT;
		case BlendOp::HslColorEXT:
			return vk::BlendOp::eHslColorEXT;
		case BlendOp::HslLuminosityEXT:
			return vk::BlendOp::eHslLuminosityEXT;
		case BlendOp::PlusEXT:
			return vk::BlendOp::ePlusEXT;
		case BlendOp::PlusClampedEXT:
			return vk::BlendOp::ePlusClampedEXT;
		case BlendOp::PlusClampedAlphaEXT:
			return vk::BlendOp::ePlusClampedAlphaEXT;
		case BlendOp::PlusDarkerEXT:
			return vk::BlendOp::ePlusDarkerEXT;
		case BlendOp::MinusEXT:
			return vk::BlendOp::eMinusEXT;
		case BlendOp::MinusClampedEXT:
			return vk::BlendOp::eMinusClampedEXT;
		case BlendOp::ContrastEXT:
			return vk::BlendOp::eContrastEXT;
		case BlendOp::InvertOvgEXT:
			return vk::BlendOp::eInvertOvgEXT;
		case BlendOp::RedEXT:
			return vk::BlendOp::eRedEXT;
		case BlendOp::GreenEXT:
			return vk::BlendOp::eGreenEXT;
		case BlendOp::BlueEXT:
			return vk::BlendOp::eBlueEXT;
	}
}

vk::ColorComponentFlags colorComponentFlagsVK(const uint16_t& _mango) {
	vk::ColorComponentFlags flags;
	if(_mango & (uint16_t)ColorComponent::eR){
		flags |= vk::ColorComponentFlagBits::eR;
	}
	if(_mango & (uint16_t)ColorComponent::eG){
		flags |= vk::ColorComponentFlagBits::eG;
	}
	if(_mango & (uint16_t)ColorComponent::eB){
		flags |= vk::ColorComponentFlagBits::eB;
	}
	if(_mango & (uint16_t)ColorComponent::eA) {
		flags |= vk::ColorComponentFlagBits::eA;
	}
	return flags;
}

vk::PipelineColorBlendAttachmentState
blendStateVK(const BlendState &_mango) {
	return vk::PipelineColorBlendAttachmentState(
				static_cast<vk::Bool32>(_mango.enable),
				blendFactorVK(_mango.srcColorBlendFactor),
				blendFactorVK(_mango.dstColorBlendFactor),
				blendOpVK(_mango.colorBlendOp),
				blendFactorVK(_mango.srcAlphaBlendFactor),
				blendFactorVK(_mango.dstAlphaBlendFactor),
				blendOpVK(_mango.alphaBlendOp),
				colorComponentFlagsVK(_mango.writeMask));
}

vk::PrimitiveTopology topologyVK(const PrimitiveTopology &_mango) {
	switch (_mango) {
		case PrimitiveTopology::PointList:
			return vk::PrimitiveTopology::ePointList;
		case PrimitiveTopology::LineList:
			return vk::PrimitiveTopology::eLineList;
		case PrimitiveTopology::LineStrip:
			return vk::PrimitiveTopology::eLineStrip;
		case PrimitiveTopology::TriangleList:
			return vk::PrimitiveTopology::eTriangleList;
		case PrimitiveTopology::TriangleStrip:
			return vk::PrimitiveTopology::eTriangleStrip;
		case PrimitiveTopology::TriangleFan:
			return vk::PrimitiveTopology::eTriangleFan;
		case PrimitiveTopology::LineListWithAdjacency:
			return vk::PrimitiveTopology::eLineListWithAdjacency;
		case PrimitiveTopology::LineStripWithAdjacency:
			return vk::PrimitiveTopology::eLineStripWithAdjacency;
		case PrimitiveTopology::TriangleListWithAdjacency:
			return vk::PrimitiveTopology::eTriangleListWithAdjacency;
		case PrimitiveTopology::TriangleStripWithAdjacency:
			return vk::PrimitiveTopology::eTriangleStripWithAdjacency;
		case PrimitiveTopology::PatchList:
			return vk::PrimitiveTopology::ePatchList;
	}
}

vk::ShaderStageFlagBits shaderStageVK(const ShaderStage &_mango) {
	switch (_mango) {
		case ShaderStage::Vertex:
			return vk::ShaderStageFlagBits::eVertex;
		case ShaderStage::TessellationControl:
			return vk::ShaderStageFlagBits::eTessellationControl;
		case ShaderStage::TessellationEvaluation:
			return vk::ShaderStageFlagBits::eTessellationEvaluation;
		case ShaderStage::Geometry:
			return vk::ShaderStageFlagBits::eGeometry;
		case ShaderStage::Fragment:
			return vk::ShaderStageFlagBits::eFragment;
		case ShaderStage::Compute:
			return vk::ShaderStageFlagBits::eCompute;
		case ShaderStage::AllGraphics:
			return vk::ShaderStageFlagBits::eAllGraphics;
		case ShaderStage::All:
			return vk::ShaderStageFlagBits::eAll;
	}
}

}
