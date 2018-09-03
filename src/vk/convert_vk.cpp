//
// Created by kurono267 on 06.07.18.
//

#include "convert_vk.hpp"

using namespace mango::vulkan;

vk::Viewport mango::vulkan::viewportVK(const mango::Viewport& _mango){
	vk::Viewport _vk;
	_vk.x = _mango.start.x; _vk.y = _mango.start.y;
	_vk.width = _mango.size.x; _vk.height = _mango.size.y;
	_vk.minDepth = _mango.minDepth; _vk.maxDepth = _mango.maxDepth;
	return _vk;
}

vk::Rect2D mango::vulkan::rect2DVK(const glm::ivec4& _mango){
	return vk::Rect2D(vk::Offset2D(_mango.x,_mango.y),vk::Extent2D(_mango.z,_mango.w));
}

vk::PolygonMode mango::vulkan::polygonModeVK(const mango::PolygonMode& _mango){
	switch (_mango){
		case mango::PolygonMode::Point:
		return vk::PolygonMode::ePoint;
		case mango::PolygonMode::Fill:
		return vk::PolygonMode::eFill;
		case mango::PolygonMode::Line:
		return vk::PolygonMode::eLine;
	}
}

vk::CullModeFlags mango::vulkan::cullModeVK(const mango::CullMode& _mango){
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

vk::FrontFace mango::vulkan::frontFaceVK(const mango::FrontFace& _mango){
	switch(_mango){
		case mango::FrontFace::CCW:
		return vk::FrontFace::eCounterClockwise;
		case mango::FrontFace::CW:
		return vk::FrontFace::eClockwise;
	}
}

vk::PipelineRasterizationStateCreateInfo mango::vulkan::rasterizationStateVK(const mango::RasterizationState& _mango){
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

vk::SampleCountFlagBits mango::vulkan::sampleCountVK(const mango::SampleCount& _mango){
	switch(_mango){
		case mango::SampleCount::s1:
		return vk::SampleCountFlagBits::e1;
		case mango::SampleCount::s2:
		return vk::SampleCountFlagBits::e2;
		case mango::SampleCount::s4:
		return vk::SampleCountFlagBits::e4;
		case mango::SampleCount::s8:
		return vk::SampleCountFlagBits::e8;
		case mango::SampleCount::s16:
		return vk::SampleCountFlagBits::e16;
		case mango::SampleCount::s32:
		return vk::SampleCountFlagBits::e32;
		case mango::SampleCount::s64:
		return vk::SampleCountFlagBits::e64;
	}
}

vk::PipelineMultisampleStateCreateInfo mango::vulkan::multisampleStateVK(const mango::MultisamplingState& _mango){
	auto _vk = vk::PipelineMultisampleStateCreateInfo(
		vk::PipelineMultisampleStateCreateFlags(),
		mango::vulkan::sampleCountVK(_mango.samples),
		static_cast<vk::Bool32>(_mango.sampleShading),
		_mango.minSampleShading,
		nullptr,
		static_cast<vk::Bool32>(_mango.alphaToCoverageEnable),
		static_cast<vk::Bool32>(_mango.alphaToOneEnable));
	return _vk;
}

vk::CompareOp mango::vulkan::compareOpVK(const mango::CompareOp& _mango){
	switch (_mango){
		case mango::CompareOp::Always:
		return vk::CompareOp::eAlways;
		case mango::CompareOp::Greater:
		return vk::CompareOp::eGreater;
		case mango::CompareOp::GreaterOrEqual:
		return vk::CompareOp::eGreaterOrEqual;
		case mango::CompareOp::Less:
		return vk::CompareOp::eLess;
		case mango::CompareOp::Never:
		return vk::CompareOp::eNever;
		case mango::CompareOp::LessOrEqual:
		return vk::CompareOp::eLessOrEqual;
		case mango::CompareOp::NotEqual:
		return vk::CompareOp::eNotEqual;
		case mango::CompareOp::Equal:
		return vk::CompareOp::eEqual;
	}
}

vk::PipelineDepthStencilStateCreateInfo mango::vulkan::depthStateVK(const mango::DepthState& _mango){
	auto _vk = vk::PipelineDepthStencilStateCreateInfo(
		vk::PipelineDepthStencilStateCreateFlags(),
		static_cast<vk::Bool32>(_mango.enable),
		static_cast<vk::Bool32>(_mango.write),
		mango::vulkan::compareOpVK(_mango.comp)
	);
	return _vk;
}

vk::BlendFactor mango::vulkan::blendFactorVK(const mango::BlendFactor& _mango){
	switch (_mango){
		case mango::BlendFactor::Zero:
		return vk::BlendFactor::eZero;
		case mango::BlendFactor::One:
		return vk::BlendFactor::eOne;
		case mango::BlendFactor::SrcColor:
		return vk::BlendFactor::eSrcColor;
		case mango::BlendFactor::OneMinusSrcColor:
		return vk::BlendFactor::eOneMinusSrcColor;
		case mango::BlendFactor::DstColor:
		return vk::BlendFactor::eDstColor;
		case mango::BlendFactor::OneMinusDstColor:
		return vk::BlendFactor::eOneMinusDstColor;
		case mango::BlendFactor::SrcAlpha:
		return vk::BlendFactor::eSrcAlpha;
		case mango::BlendFactor::OneMinusSrcAlpha:
		return vk::BlendFactor::eOneMinusSrcAlpha;
		case mango::BlendFactor::DstAlpha:
		return vk::BlendFactor::eDstAlpha;
		case mango::BlendFactor::OneMinusDstAlpha:
		return vk::BlendFactor::eOneMinusDstAlpha;
		case mango::BlendFactor::ConstantColor:
		return vk::BlendFactor::eConstantColor;
		case mango::BlendFactor::OneMinusConstantColor:
		return vk::BlendFactor::eOneMinusConstantColor;
		case mango::BlendFactor::ConstantAlpha:
		return vk::BlendFactor::eConstantAlpha;
		case mango::BlendFactor::OneMinusConstantAlpha:
		return vk::BlendFactor::eOneMinusConstantAlpha;
		case mango::BlendFactor::SrcAlphaSaturate:
		return vk::BlendFactor::eSrcAlphaSaturate;
		case mango::BlendFactor::Src1Color:
		return vk::BlendFactor::eSrc1Color;
		case mango::BlendFactor::OneMinusSrc1Color:
		return vk::BlendFactor::eOneMinusSrc1Color;
		case mango::BlendFactor::Src1Alpha:
		return vk::BlendFactor::eSrc1Alpha;
		case mango::BlendFactor::OneMinusSrc1Alpha:
		return vk::BlendFactor::eOneMinusSrc1Alpha;
	}
}

vk::BlendOp mango::vulkan::blendOpVK(const mango::BlendOp& _mango){
	switch (_mango){
		case mango::BlendOp::Add:return vk::BlendOp::eAdd;
		case mango::BlendOp::Subtract:return vk::BlendOp::eSubtract;
		case mango::BlendOp::ReverseSubtract:return vk::BlendOp::eReverseSubtract;
		case mango::BlendOp::Min:return vk::BlendOp::eMin;
		case mango::BlendOp::Max:return vk::BlendOp::eMax;
		case mango::BlendOp::ZeroEXT:return vk::BlendOp::eZeroEXT;
		case mango::BlendOp::SrcEXT:return vk::BlendOp::eSrcEXT;
		case mango::BlendOp::DstEXT:return vk::BlendOp::eDstEXT;
		case mango::BlendOp::SrcOverEXT:return vk::BlendOp::eSrcOverEXT;
		case mango::BlendOp::DstOverEXT:return vk::BlendOp::eDstOverEXT;
		case mango::BlendOp::SrcInEXT:return vk::BlendOp::eSrcInEXT;
		case mango::BlendOp::DstInEXT:return vk::BlendOp::eDstInEXT;
		case mango::BlendOp::SrcOutEXT:return vk::BlendOp::eSrcOutEXT;
		case mango::BlendOp::DstOutEXT:return vk::BlendOp::eDstOutEXT;
		case mango::BlendOp::SrcAtopEXT:return vk::BlendOp::eSrcAtopEXT;
		case mango::BlendOp::DstAtopEXT:return vk::BlendOp::eDstAtopEXT;
		case mango::BlendOp::XorEXT:return vk::BlendOp::eXorEXT;
		case mango::BlendOp::MultiplyEXT:return vk::BlendOp::eMultiplyEXT;
		case mango::BlendOp::ScreenEXT:return vk::BlendOp::eScreenEXT;
		case mango::BlendOp::OverlayEXT:return vk::BlendOp::eOverlayEXT;
		case mango::BlendOp::DarkenEXT:return vk::BlendOp::eDarkenEXT;
		case mango::BlendOp::LightenEXT:return vk::BlendOp::eLightenEXT;
		case mango::BlendOp::ColordodgeEXT:return vk::BlendOp::eColordodgeEXT;
		case mango::BlendOp::ColorburnEXT:return vk::BlendOp::eColorburnEXT;
		case mango::BlendOp::HardlightEXT:return vk::BlendOp::eHardlightEXT;
		case mango::BlendOp::SoftlightEXT:return vk::BlendOp::eSoftlightEXT;
		case mango::BlendOp::DifferenceEXT:return vk::BlendOp::eDifferenceEXT;
		case mango::BlendOp::ExclusionEXT:return vk::BlendOp::eExclusionEXT;
		case mango::BlendOp::InvertEXT:return vk::BlendOp::eInvertEXT;
		case mango::BlendOp::InvertRgbEXT:return vk::BlendOp::eInvertRgbEXT;
		case mango::BlendOp::LineardodgeEXT:return vk::BlendOp::eLineardodgeEXT;
		case mango::BlendOp::LinearburnEXT:return vk::BlendOp::eLinearburnEXT;
		case mango::BlendOp::VividlightEXT:return vk::BlendOp::eVividlightEXT;
		case mango::BlendOp::LinearlightEXT:return vk::BlendOp::eLinearlightEXT;
		case mango::BlendOp::PinlightEXT:return vk::BlendOp::ePinlightEXT;
		case mango::BlendOp::HardmixEXT:return vk::BlendOp::eHardmixEXT;
		case mango::BlendOp::HslHueEXT:return vk::BlendOp::eHslHueEXT;
		case mango::BlendOp::HslSaturationEXT:return vk::BlendOp::eHslSaturationEXT;
		case mango::BlendOp::HslColorEXT:return vk::BlendOp::eHslColorEXT;
		case mango::BlendOp::HslLuminosityEXT:return vk::BlendOp::eHslLuminosityEXT;
		case mango::BlendOp::PlusEXT:return vk::BlendOp::ePlusEXT;
		case mango::BlendOp::PlusClampedEXT:return vk::BlendOp::ePlusClampedEXT;
		case mango::BlendOp::PlusClampedAlphaEXT:return vk::BlendOp::ePlusClampedAlphaEXT;
		case mango::BlendOp::PlusDarkerEXT:return vk::BlendOp::ePlusDarkerEXT;
		case mango::BlendOp::MinusEXT:return vk::BlendOp::eMinusEXT;
		case mango::BlendOp::MinusClampedEXT:return vk::BlendOp::eMinusClampedEXT;
		case mango::BlendOp::ContrastEXT:return vk::BlendOp::eContrastEXT;
		case mango::BlendOp::InvertOvgEXT:return vk::BlendOp::eInvertOvgEXT;
		case mango::BlendOp::RedEXT:return vk::BlendOp::eRedEXT;
		case mango::BlendOp::GreenEXT:return vk::BlendOp::eGreenEXT;
		case mango::BlendOp::BlueEXT:return vk::BlendOp::eBlueEXT;
	}
}

vk::ColorComponentFlagBits mango::vulkan::colorComponenetBitVK(const mango::ColorComponent& _mango){
	switch(_mango){
		case mango::ColorComponent::eR:return vk::ColorComponentFlagBits::eR;
		case mango::ColorComponent::eG:return vk::ColorComponentFlagBits::eG;
		case mango::ColorComponent::eB :return vk::ColorComponentFlagBits::eB;
		case mango::ColorComponent::eA:return vk::ColorComponentFlagBits::eA;
	}
}

vk::ColorComponentFlags mango::vulkan::colorComponentFlagsVK(const std::vector<mango::ColorComponent>& _mango){
	vk::ColorComponentFlags flags = mango::vulkan::colorComponenetBitVK(_mango[0]);
	for(int i = 1;i<_mango.size();++i){
		flags |= mango::vulkan::colorComponenetBitVK(_mango[i]);
	}
	return flags;
}

vk::PipelineColorBlendStateCreateInfo mango::vulkan::blendStateVK(const std::vector<mango::BlendAttachmentState>& _mango){
	vk::PipelineColorBlendAttachmentState* blendAttachments = new vk::PipelineColorBlendAttachmentState[_mango.size()];
	for(int i = 0;i<_mango.size();++i){
		blendAttachments[i] = vk::PipelineColorBlendAttachmentState(
				static_cast<vk::Bool32>(_mango[i].enable),
			mango::vulkan::blendFactorVK(_mango[i].srcColorBlendFactor),mango::vulkan::blendFactorVK(_mango[i].dstColorBlendFactor),
			mango::vulkan::blendOpVK(_mango[i].colorBlendOp),
			mango::vulkan::blendFactorVK(_mango[i].srcAlphaBlendFactor),mango::vulkan::blendFactorVK(_mango[i].dstAlphaBlendFactor),
			mango::vulkan::blendOpVK(_mango[i].alphaBlendOp),
			mango::vulkan::colorComponentFlagsVK(_mango[i].writeMask));
	}

	return vk::PipelineColorBlendStateCreateInfo(vk::PipelineColorBlendStateCreateFlags(),0,vk::LogicOp::eCopy,_mango.size(),blendAttachments);
}

vk::PrimitiveTopology mango::vulkan::topologyVK(const mango::PrimitiveTopology& _mango){
	switch (_mango){
		case mango::PrimitiveTopology::PointList:return vk::PrimitiveTopology::ePointList;
		case mango::PrimitiveTopology::LineList:return vk::PrimitiveTopology::eLineList;
		case mango::PrimitiveTopology::LineStrip:return vk::PrimitiveTopology::eLineStrip;
		case mango::PrimitiveTopology::TriangleList:return vk::PrimitiveTopology::eTriangleList;
		case mango::PrimitiveTopology::TriangleStrip:return vk::PrimitiveTopology::eTriangleStrip;
		case mango::PrimitiveTopology::TriangleFan:return vk::PrimitiveTopology::eTriangleFan;
		case mango::PrimitiveTopology::LineListWithAdjacency:return vk::PrimitiveTopology::eLineListWithAdjacency;
		case mango::PrimitiveTopology::LineStripWithAdjacency:return vk::PrimitiveTopology::eLineStripWithAdjacency;
		case mango::PrimitiveTopology::TriangleListWithAdjacency:return vk::PrimitiveTopology::eTriangleListWithAdjacency;
		case mango::PrimitiveTopology::TriangleStripWithAdjacency:return vk::PrimitiveTopology::eTriangleStripWithAdjacency;
		case mango::PrimitiveTopology::PatchList:return vk::PrimitiveTopology::ePatchList;
	}
}

vk::Format mango::vulkan::formatVK(const mango::Format& _mango){
	switch (_mango){
		case mango::Format::Undefined:return vk::Format::eUndefined;
		case mango::Format::R4G4UnormPack8:return vk::Format::eR4G4UnormPack8;
		case mango::Format::R4G4B4A4UnormPack16:return vk::Format::eR4G4B4A4UnormPack16;
		case mango::Format::B4G4R4A4UnormPack16:return vk::Format::eB4G4R4A4UnormPack16;
		case mango::Format::R5G6B5UnormPack16:return vk::Format::eR5G6B5UnormPack16;
		case mango::Format::B5G6R5UnormPack16:return vk::Format::eB5G6R5UnormPack16;
		case mango::Format::R5G5B5A1UnormPack16:return vk::Format::eR5G5B5A1UnormPack16;
		case mango::Format::B5G5R5A1UnormPack16:return vk::Format::eB5G5R5A1UnormPack16;
		case mango::Format::A1R5G5B5UnormPack16:return vk::Format::eA1R5G5B5UnormPack16;
		case mango::Format::R8Unorm:return vk::Format::eR8Unorm;
		case mango::Format::R8Snorm:return vk::Format::eR8Snorm;
		case mango::Format::R8Uscaled:return vk::Format::eR8Uscaled;
		case mango::Format::R8Sscaled:return vk::Format::eR8Sscaled;
		case mango::Format::R8Uint:return vk::Format::eR8Uint;
		case mango::Format::R8Sint:return vk::Format::eR8Sint;
		case mango::Format::R8Srgb:return vk::Format::eR8Srgb;
		case mango::Format::R8G8Unorm:return vk::Format::eR8G8Unorm;
		case mango::Format::R8G8Snorm:return vk::Format::eR8G8Snorm;
		case mango::Format::R8G8Uscaled:return vk::Format::eR8G8Uscaled;
		case mango::Format::R8G8Sscaled:return vk::Format::eR8G8Sscaled;
		case mango::Format::R8G8Uint:return vk::Format::eR8G8Uint;
		case mango::Format::R8G8Sint:return vk::Format::eR8G8Sint;
		case mango::Format::R8G8Srgb:return vk::Format::eR8G8Srgb;
		case mango::Format::R8G8B8Unorm:return vk::Format::eR8G8B8Unorm;
		case mango::Format::R8G8B8Snorm:return vk::Format::eR8G8B8Snorm;
		case mango::Format::R8G8B8Uscaled:return vk::Format::eR8G8B8Uscaled;
		case mango::Format::R8G8B8Sscaled:return vk::Format::eR8G8B8Sscaled;
		case mango::Format::R8G8B8Uint:return vk::Format::eR8G8B8Uint;
		case mango::Format::R8G8B8Sint:return vk::Format::eR8G8B8Sint;
		case mango::Format::R8G8B8Srgb:return vk::Format::eR8G8B8Srgb;
		case mango::Format::B8G8R8Unorm:return vk::Format::eB8G8R8Unorm;
		case mango::Format::B8G8R8Snorm:return vk::Format::eB8G8R8Snorm;
		case mango::Format::B8G8R8Uscaled:return vk::Format::eB8G8R8Uscaled;
		case mango::Format::B8G8R8Sscaled:return vk::Format::eB8G8R8Sscaled;
		case mango::Format::B8G8R8Uint:return vk::Format::eB8G8R8Uint;
		case mango::Format::B8G8R8Sint:return vk::Format::eB8G8R8Sint;
		case mango::Format::B8G8R8Srgb:return vk::Format::eB8G8R8Srgb;
		case mango::Format::R8G8B8A8Unorm:return vk::Format::eR8G8B8A8Unorm;
		case mango::Format::R8G8B8A8Snorm:return vk::Format::eR8G8B8A8Snorm;
		case mango::Format::R8G8B8A8Uscaled:return vk::Format::eR8G8B8A8Uscaled;
		case mango::Format::R8G8B8A8Sscaled:return vk::Format::eR8G8B8A8Sscaled;
		case mango::Format::R8G8B8A8Uint:return vk::Format::eR8G8B8A8Uint;
		case mango::Format::R8G8B8A8Sint:return vk::Format::eR8G8B8A8Sint;
		case mango::Format::R8G8B8A8Srgb:return vk::Format::eR8G8B8A8Srgb;
		case mango::Format::B8G8R8A8Unorm:return vk::Format::eB8G8R8A8Unorm;
		case mango::Format::B8G8R8A8Snorm:return vk::Format::eB8G8R8A8Snorm;
		case mango::Format::B8G8R8A8Uscaled:return vk::Format::eB8G8R8A8Uscaled;
		case mango::Format::B8G8R8A8Sscaled:return vk::Format::eB8G8R8A8Sscaled;
		case mango::Format::B8G8R8A8Uint:return vk::Format::eB8G8R8A8Uint;
		case mango::Format::B8G8R8A8Sint:return vk::Format::eB8G8R8A8Sint;
		case mango::Format::B8G8R8A8Srgb:return vk::Format::eB8G8R8A8Srgb;
		case mango::Format::A8B8G8R8UnormPack32:return vk::Format::eA8B8G8R8UnormPack32;
		case mango::Format::A8B8G8R8SnormPack32:return vk::Format::eA8B8G8R8SnormPack32;
		case mango::Format::A8B8G8R8UscaledPack32:return vk::Format::eA8B8G8R8UscaledPack32;
		case mango::Format::A8B8G8R8SscaledPack32:return vk::Format::eA8B8G8R8SscaledPack32;
		case mango::Format::A8B8G8R8UintPack32:return vk::Format::eA8B8G8R8UintPack32;
		case mango::Format::A8B8G8R8SintPack32:return vk::Format::eA8B8G8R8SintPack32;
		case mango::Format::A8B8G8R8SrgbPack32:return vk::Format::eA8B8G8R8SrgbPack32;
		case mango::Format::A2R10G10B10UnormPack32:return vk::Format::eA2R10G10B10UnormPack32;
		case mango::Format::A2R10G10B10SnormPack32:return vk::Format::eA2R10G10B10SnormPack32;
		case mango::Format::A2R10G10B10UscaledPack32:return vk::Format::eA2R10G10B10UscaledPack32;
		case mango::Format::A2R10G10B10SscaledPack32:return vk::Format::eA2R10G10B10SscaledPack32;
		case mango::Format::A2R10G10B10UintPack32:return vk::Format::eA2R10G10B10UintPack32;
		case mango::Format::A2R10G10B10SintPack32:return vk::Format::eA2R10G10B10SintPack32;
		case mango::Format::A2B10G10R10UnormPack32:return vk::Format::eA2B10G10R10UnormPack32;
		case mango::Format::A2B10G10R10SnormPack32:return vk::Format::eA2B10G10R10SnormPack32;
		case mango::Format::A2B10G10R10UscaledPack32:return vk::Format::eA2B10G10R10UscaledPack32;
		case mango::Format::A2B10G10R10SscaledPack32:return vk::Format::eA2B10G10R10SscaledPack32;
		case mango::Format::A2B10G10R10UintPack32:return vk::Format::eA2B10G10R10UintPack32;
		case mango::Format::A2B10G10R10SintPack32:return vk::Format::eA2B10G10R10SintPack32;
		case mango::Format::R16Unorm:return vk::Format::eR16Unorm;
		case mango::Format::R16Snorm:return vk::Format::eR16Snorm;
		case mango::Format::R16Uscaled:return vk::Format::eR16Uscaled;
		case mango::Format::R16Sscaled:return vk::Format::eR16Sscaled;
		case mango::Format::R16Uint:return vk::Format::eR16Uint;
		case mango::Format::R16Sint:return vk::Format::eR16Sint;
		case mango::Format::R16Sfloat:return vk::Format::eR16Sfloat;
		case mango::Format::R16G16Unorm:return vk::Format::eR16G16Unorm;
		case mango::Format::R16G16Snorm:return vk::Format::eR16G16Snorm;
		case mango::Format::R16G16Uscaled:return vk::Format::eR16G16Uscaled;
		case mango::Format::R16G16Sscaled:return vk::Format::eR16G16Sscaled;
		case mango::Format::R16G16Uint:return vk::Format::eR16G16Uint;
		case mango::Format::R16G16Sint:return vk::Format::eR16G16Sint;
		case mango::Format::R16G16Sfloat:return vk::Format::eR16G16Sfloat;
		case mango::Format::R16G16B16Unorm:return vk::Format::eR16G16B16Unorm;
		case mango::Format::R16G16B16Snorm:return vk::Format::eR16G16B16Snorm;
		case mango::Format::R16G16B16Uscaled:return vk::Format::eR16G16B16Uscaled;
		case mango::Format::R16G16B16Sscaled:return vk::Format::eR16G16B16Sscaled;
		case mango::Format::R16G16B16Uint:return vk::Format::eR16G16B16Uint;
		case mango::Format::R16G16B16Sint:return vk::Format::eR16G16B16Sint;
		case mango::Format::R16G16B16Sfloat:return vk::Format::eR16G16B16Sfloat;
		case mango::Format::R16G16B16A16Unorm:return vk::Format::eR16G16B16A16Unorm;
		case mango::Format::R16G16B16A16Snorm:return vk::Format::eR16G16B16A16Snorm;
		case mango::Format::R16G16B16A16Uscaled:return vk::Format::eR16G16B16A16Uscaled;
		case mango::Format::R16G16B16A16Sscaled:return vk::Format::eR16G16B16A16Sscaled;
		case mango::Format::R16G16B16A16Uint:return vk::Format::eR16G16B16A16Uint;
		case mango::Format::R16G16B16A16Sint:return vk::Format::eR16G16B16A16Sint;
		case mango::Format::R16G16B16A16Sfloat:return vk::Format::eR16G16B16A16Sfloat;
		case mango::Format::R32Uint:return vk::Format::eR32Uint;
		case mango::Format::R32Sint:return vk::Format::eR32Sint;
		case mango::Format::R32Sfloat:return vk::Format::eR32Sfloat;
		case mango::Format::R32G32Uint:return vk::Format::eR32G32Uint;
		case mango::Format::R32G32Sint:return vk::Format::eR32G32Sint;
		case mango::Format::R32G32Sfloat:return vk::Format::eR32G32Sfloat;
		case mango::Format::R32G32B32Uint:return vk::Format::eR32G32B32Uint;
		case mango::Format::R32G32B32Sint:return vk::Format::eR32G32B32Sint;
		case mango::Format::R32G32B32Sfloat:return vk::Format::eR32G32B32Sfloat;
		case mango::Format::R32G32B32A32Uint:return vk::Format::eR32G32B32A32Uint;
		case mango::Format::R32G32B32A32Sint:return vk::Format::eR32G32B32A32Sint;
		case mango::Format::R32G32B32A32Sfloat:return vk::Format::eR32G32B32A32Sfloat;
		case mango::Format::R64Uint:return vk::Format::eR64Uint;
		case mango::Format::R64Sint:return vk::Format::eR64Sint;
		case mango::Format::R64Sfloat:return vk::Format::eR64Sfloat;
		case mango::Format::R64G64Uint:return vk::Format::eR64G64Uint;
		case mango::Format::R64G64Sint:return vk::Format::eR64G64Sint;
		case mango::Format::R64G64Sfloat:return vk::Format::eR64G64Sfloat;
		case mango::Format::R64G64B64Uint:return vk::Format::eR64G64B64Uint;
		case mango::Format::R64G64B64Sint:return vk::Format::eR64G64B64Sint;
		case mango::Format::R64G64B64Sfloat:return vk::Format::eR64G64B64Sfloat;
		case mango::Format::R64G64B64A64Uint:return vk::Format::eR64G64B64A64Uint;
		case mango::Format::R64G64B64A64Sint:return vk::Format::eR64G64B64A64Sint;
		case mango::Format::R64G64B64A64Sfloat:return vk::Format::eR64G64B64A64Sfloat;
		case mango::Format::B10G11R11UfloatPack32:return vk::Format::eB10G11R11UfloatPack32;
		case mango::Format::E5B9G9R9UfloatPack32:return vk::Format::eE5B9G9R9UfloatPack32;
		case mango::Format::D16Unorm:return vk::Format::eD16Unorm;
		case mango::Format::X8D24UnormPack32:return vk::Format::eX8D24UnormPack32;
		case mango::Format::D32Sfloat:return vk::Format::eD32Sfloat;
		case mango::Format::S8Uint:return vk::Format::eS8Uint;
		case mango::Format::D16UnormS8Uint:return vk::Format::eD16UnormS8Uint;
		case mango::Format::D24UnormS8Uint:return vk::Format::eD24UnormS8Uint;
		case mango::Format::D32SfloatS8Uint:return vk::Format::eD32SfloatS8Uint;
		case mango::Format::Bc1RgbUnormBlock:return vk::Format::eBc1RgbUnormBlock;
		case mango::Format::Bc1RgbSrgbBlock:return vk::Format::eBc1RgbSrgbBlock;
		case mango::Format::Bc1RgbaUnormBlock:return vk::Format::eBc1RgbaUnormBlock;
		case mango::Format::Bc1RgbaSrgbBlock:return vk::Format::eBc1RgbaSrgbBlock;
		case mango::Format::Bc2UnormBlock:return vk::Format::eBc2UnormBlock;
		case mango::Format::Bc2SrgbBlock:return vk::Format::eBc2SrgbBlock;
		case mango::Format::Bc3UnormBlock:return vk::Format::eBc3UnormBlock;
		case mango::Format::Bc3SrgbBlock:return vk::Format::eBc3SrgbBlock;
		case mango::Format::Bc4UnormBlock:return vk::Format::eBc4UnormBlock;
		case mango::Format::Bc4SnormBlock:return vk::Format::eBc4SnormBlock;
		case mango::Format::Bc5UnormBlock:return vk::Format::eBc5UnormBlock;
		case mango::Format::Bc5SnormBlock:return vk::Format::eBc5SnormBlock;
		case mango::Format::Bc6HUfloatBlock:return vk::Format::eBc6HUfloatBlock;
		case mango::Format::Bc6HSfloatBlock:return vk::Format::eBc6HSfloatBlock;
		case mango::Format::Bc7UnormBlock:return vk::Format::eBc7UnormBlock;
		case mango::Format::Bc7SrgbBlock:return vk::Format::eBc7SrgbBlock;
		case mango::Format::Etc2R8G8B8UnormBlock:return vk::Format::eEtc2R8G8B8UnormBlock;
		case mango::Format::Etc2R8G8B8SrgbBlock:return vk::Format::eEtc2R8G8B8SrgbBlock;
		case mango::Format::Etc2R8G8B8A1UnormBlock:return vk::Format::eEtc2R8G8B8A1UnormBlock;
		case mango::Format::Etc2R8G8B8A1SrgbBlock:return vk::Format::eEtc2R8G8B8A1SrgbBlock;
		case mango::Format::Etc2R8G8B8A8UnormBlock:return vk::Format::eEtc2R8G8B8A8UnormBlock;
		case mango::Format::Etc2R8G8B8A8SrgbBlock:return vk::Format::eEtc2R8G8B8A8SrgbBlock;
		case mango::Format::EacR11UnormBlock:return vk::Format::eEacR11UnormBlock;
		case mango::Format::EacR11SnormBlock:return vk::Format::eEacR11SnormBlock;
		case mango::Format::EacR11G11UnormBlock:return vk::Format::eEacR11G11UnormBlock;
		case mango::Format::EacR11G11SnormBlock:return vk::Format::eEacR11G11SnormBlock;
		case mango::Format::Astc4x4UnormBlock:return vk::Format::eAstc4x4UnormBlock;
		case mango::Format::Astc4x4SrgbBlock:return vk::Format::eAstc4x4SrgbBlock;
		case mango::Format::Astc5x4UnormBlock:return vk::Format::eAstc5x4UnormBlock;
		case mango::Format::Astc5x4SrgbBlock:return vk::Format::eAstc5x4SrgbBlock;
		case mango::Format::Astc5x5UnormBlock:return vk::Format::eAstc5x5UnormBlock;
		case mango::Format::Astc5x5SrgbBlock:return vk::Format::eAstc5x5SrgbBlock;
		case mango::Format::Astc6x5UnormBlock:return vk::Format::eAstc6x5UnormBlock;
		case mango::Format::Astc6x5SrgbBlock:return vk::Format::eAstc6x5SrgbBlock;
		case mango::Format::Astc6x6UnormBlock:return vk::Format::eAstc6x6UnormBlock;
		case mango::Format::Astc6x6SrgbBlock:return vk::Format::eAstc6x6SrgbBlock;
		case mango::Format::Astc8x5UnormBlock:return vk::Format::eAstc8x5UnormBlock;
		case mango::Format::Astc8x5SrgbBlock:return vk::Format::eAstc8x5SrgbBlock;
		case mango::Format::Astc8x6UnormBlock:return vk::Format::eAstc8x6UnormBlock;
		case mango::Format::Astc8x6SrgbBlock:return vk::Format::eAstc8x6SrgbBlock;
		case mango::Format::Astc8x8UnormBlock:return vk::Format::eAstc8x8UnormBlock;
		case mango::Format::Astc8x8SrgbBlock:return vk::Format::eAstc8x8SrgbBlock;
		case mango::Format::Astc10x5UnormBlock:return vk::Format::eAstc10x5UnormBlock;
		case mango::Format::Astc10x5SrgbBlock:return vk::Format::eAstc10x5SrgbBlock;
		case mango::Format::Astc10x6UnormBlock:return vk::Format::eAstc10x6UnormBlock;
		case mango::Format::Astc10x6SrgbBlock:return vk::Format::eAstc10x6SrgbBlock;
		case mango::Format::Astc10x8UnormBlock:return vk::Format::eAstc10x8UnormBlock;
		case mango::Format::Astc10x8SrgbBlock:return vk::Format::eAstc10x8SrgbBlock;
		case mango::Format::Astc10x10UnormBlock:return vk::Format::eAstc10x10UnormBlock;
		case mango::Format::Astc10x10SrgbBlock:return vk::Format::eAstc10x10SrgbBlock;
		case mango::Format::Astc12x10UnormBlock:return vk::Format::eAstc12x10UnormBlock;
		case mango::Format::Astc12x10SrgbBlock:return vk::Format::eAstc12x10SrgbBlock;
		case mango::Format::Astc12x12UnormBlock:return vk::Format::eAstc12x12UnormBlock;
		case mango::Format::Astc12x12SrgbBlock:return vk::Format::eAstc12x12SrgbBlock;
		case mango::Format::G8B8G8R8422Unorm:return vk::Format::eG8B8G8R8422Unorm;
		case mango::Format::G8B8G8R8422UnormKHR:return vk::Format::eG8B8G8R8422UnormKHR;
		case mango::Format::B8G8R8G8422Unorm:return vk::Format::eB8G8R8G8422Unorm;
		case mango::Format::B8G8R8G8422UnormKHR:return vk::Format::eB8G8R8G8422UnormKHR;
		case mango::Format::G8B8R83Plane420Unorm:return vk::Format::eG8B8R83Plane420Unorm;
		case mango::Format::G8B8R83Plane420UnormKHR:return vk::Format::eG8B8R83Plane420UnormKHR;
		case mango::Format::G8B8R82Plane420Unorm:return vk::Format::eG8B8R82Plane420Unorm;
		case mango::Format::G8B8R82Plane420UnormKHR:return vk::Format::eG8B8R82Plane420UnormKHR;
		case mango::Format::G8B8R83Plane422Unorm:return vk::Format::eG8B8R83Plane422Unorm;
		case mango::Format::G8B8R83Plane422UnormKHR:return vk::Format::eG8B8R83Plane422UnormKHR;
		case mango::Format::G8B8R82Plane422Unorm:return vk::Format::eG8B8R82Plane422Unorm;
		case mango::Format::G8B8R82Plane422UnormKHR:return vk::Format::eG8B8R82Plane422UnormKHR;
		case mango::Format::G8B8R83Plane444Unorm:return vk::Format::eG8B8R83Plane444Unorm;
		case mango::Format::G8B8R83Plane444UnormKHR:return vk::Format::eG8B8R83Plane444UnormKHR;
		case mango::Format::R10X6UnormPack16:return vk::Format::eR10X6UnormPack16;
		case mango::Format::R10X6UnormPack16KHR:return vk::Format::eR10X6UnormPack16KHR;
		case mango::Format::R10X6G10X6Unorm2Pack16:return vk::Format::eR10X6G10X6Unorm2Pack16;
		case mango::Format::R10X6G10X6Unorm2Pack16KHR:return vk::Format::eR10X6G10X6Unorm2Pack16KHR;
		case mango::Format::R10X6G10X6B10X6A10X6Unorm4Pack16:return vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16;
		case mango::Format::R10X6G10X6B10X6A10X6Unorm4Pack16KHR:return vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16KHR;
		case mango::Format::G10X6B10X6G10X6R10X6422Unorm4Pack16:return vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16;
		case mango::Format::G10X6B10X6G10X6R10X6422Unorm4Pack16KHR:return vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16KHR;
		case mango::Format::B10X6G10X6R10X6G10X6422Unorm4Pack16:return vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16;
		case mango::Format::B10X6G10X6R10X6G10X6422Unorm4Pack16KHR:return vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16KHR;
		case mango::Format::G10X6B10X6R10X63Plane420Unorm3Pack16:return vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16;
		case mango::Format::G10X6B10X6R10X63Plane420Unorm3Pack16KHR:return vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16KHR;
		case mango::Format::G10X6B10X6R10X62Plane420Unorm3Pack16:return vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16;
		case mango::Format::G10X6B10X6R10X62Plane420Unorm3Pack16KHR:return vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16KHR;
		case mango::Format::G10X6B10X6R10X63Plane422Unorm3Pack16:return vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16;
		case mango::Format::G10X6B10X6R10X63Plane422Unorm3Pack16KHR:return vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16KHR;
		case mango::Format::G10X6B10X6R10X62Plane422Unorm3Pack16:return vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16;
		case mango::Format::G10X6B10X6R10X62Plane422Unorm3Pack16KHR:return vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16KHR;
		case mango::Format::G10X6B10X6R10X63Plane444Unorm3Pack16:return vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16;
		case mango::Format::G10X6B10X6R10X63Plane444Unorm3Pack16KHR:return vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16KHR;
		case mango::Format::R12X4UnormPack16:return vk::Format::eR12X4UnormPack16;
		case mango::Format::R12X4UnormPack16KHR:return vk::Format::eR12X4UnormPack16KHR;
		case mango::Format::R12X4G12X4Unorm2Pack16:return vk::Format::eR12X4G12X4Unorm2Pack16;
		case mango::Format::R12X4G12X4Unorm2Pack16KHR:return vk::Format::eR12X4G12X4Unorm2Pack16KHR;
		case mango::Format::R12X4G12X4B12X4A12X4Unorm4Pack16:return vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16;
		case mango::Format::R12X4G12X4B12X4A12X4Unorm4Pack16KHR:return vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16KHR;
		case mango::Format::G12X4B12X4G12X4R12X4422Unorm4Pack16:return vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16;
		case mango::Format::G12X4B12X4G12X4R12X4422Unorm4Pack16KHR:return vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16KHR;
		case mango::Format::B12X4G12X4R12X4G12X4422Unorm4Pack16:return vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16;
		case mango::Format::B12X4G12X4R12X4G12X4422Unorm4Pack16KHR:return vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16KHR;
		case mango::Format::G12X4B12X4R12X43Plane420Unorm3Pack16:return vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16;
		case mango::Format::G12X4B12X4R12X43Plane420Unorm3Pack16KHR:return vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16KHR;
		case mango::Format::G12X4B12X4R12X42Plane420Unorm3Pack16:return vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16;
		case mango::Format::G12X4B12X4R12X42Plane420Unorm3Pack16KHR:return vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16KHR;
		case mango::Format::G12X4B12X4R12X43Plane422Unorm3Pack16:return vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16;
		case mango::Format::G12X4B12X4R12X43Plane422Unorm3Pack16KHR:return vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16KHR;
		case mango::Format::G12X4B12X4R12X42Plane422Unorm3Pack16:return vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16;
		case mango::Format::G12X4B12X4R12X42Plane422Unorm3Pack16KHR:return vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16KHR;
		case mango::Format::G12X4B12X4R12X43Plane444Unorm3Pack16:return vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16;
		case mango::Format::G12X4B12X4R12X43Plane444Unorm3Pack16KHR:return vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16KHR;
		case mango::Format::G16B16G16R16422Unorm:return vk::Format::eG16B16G16R16422Unorm;
		case mango::Format::G16B16G16R16422UnormKHR:return vk::Format::eG16B16G16R16422UnormKHR;
		case mango::Format::B16G16R16G16422Unorm:return vk::Format::eB16G16R16G16422Unorm;
		case mango::Format::B16G16R16G16422UnormKHR:return vk::Format::eB16G16R16G16422UnormKHR;
		case mango::Format::G16B16R163Plane420Unorm:return vk::Format::eG16B16R163Plane420Unorm;
		case mango::Format::G16B16R163Plane420UnormKHR:return vk::Format::eG16B16R163Plane420UnormKHR;
		case mango::Format::G16B16R162Plane420Unorm:return vk::Format::eG16B16R162Plane420Unorm;
		case mango::Format::G16B16R162Plane420UnormKHR:return vk::Format::eG16B16R162Plane420UnormKHR;
		case mango::Format::G16B16R163Plane422Unorm:return vk::Format::eG16B16R163Plane422Unorm;
		case mango::Format::G16B16R163Plane422UnormKHR:return vk::Format::eG16B16R163Plane422UnormKHR;
		case mango::Format::G16B16R162Plane422Unorm:return vk::Format::eG16B16R162Plane422Unorm;
		case mango::Format::G16B16R162Plane422UnormKHR:return vk::Format::eG16B16R162Plane422UnormKHR;
		case mango::Format::G16B16R163Plane444Unorm:return vk::Format::eG16B16R163Plane444Unorm;
		case mango::Format::G16B16R163Plane444UnormKHR:return vk::Format::eG16B16R163Plane444UnormKHR;
		case mango::Format::Pvrtc12BppUnormBlockIMG:return vk::Format::ePvrtc12BppUnormBlockIMG;
		case mango::Format::Pvrtc14BppUnormBlockIMG:return vk::Format::ePvrtc14BppUnormBlockIMG;
		case mango::Format::Pvrtc22BppUnormBlockIMG:return vk::Format::ePvrtc22BppUnormBlockIMG;
		case mango::Format::Pvrtc24BppUnormBlockIMG:return vk::Format::ePvrtc24BppUnormBlockIMG;
		case mango::Format::Pvrtc12BppSrgbBlockIMG:return vk::Format::ePvrtc12BppSrgbBlockIMG;
		case mango::Format::Pvrtc14BppSrgbBlockIMG:return vk::Format::ePvrtc14BppSrgbBlockIMG;
		case mango::Format::Pvrtc22BppSrgbBlockIMG:return vk::Format::ePvrtc22BppSrgbBlockIMG;
		case mango::Format::Pvrtc24BppSrgbBlockIMG:return vk::Format::ePvrtc24BppSrgbBlockIMG;
	}
}

mango::Format mango::vulkan::formatVK2Mango(const vk::Format& _vk){
	switch (_vk){
		case vk::Format::eUndefined: return mango::Format::Undefined;
		case vk::Format::eR4G4UnormPack8: return mango::Format::R4G4UnormPack8;
		case vk::Format::eR4G4B4A4UnormPack16: return mango::Format::R4G4B4A4UnormPack16;
		case vk::Format::eB4G4R4A4UnormPack16: return mango::Format::B4G4R4A4UnormPack16;
		case vk::Format::eR5G6B5UnormPack16: return mango::Format::R5G6B5UnormPack16;
		case vk::Format::eB5G6R5UnormPack16: return mango::Format::B5G6R5UnormPack16;
		case vk::Format::eR5G5B5A1UnormPack16: return mango::Format::R5G5B5A1UnormPack16;
		case vk::Format::eB5G5R5A1UnormPack16: return mango::Format::B5G5R5A1UnormPack16;
		case vk::Format::eA1R5G5B5UnormPack16: return mango::Format::A1R5G5B5UnormPack16;
		case vk::Format::eR8Unorm: return mango::Format::R8Unorm;
		case vk::Format::eR8Snorm: return mango::Format::R8Snorm;
		case vk::Format::eR8Uscaled: return mango::Format::R8Uscaled;
		case vk::Format::eR8Sscaled: return mango::Format::R8Sscaled;
		case vk::Format::eR8Uint: return mango::Format::R8Uint;
		case vk::Format::eR8Sint: return mango::Format::R8Sint;
		case vk::Format::eR8Srgb: return mango::Format::R8Srgb;
		case vk::Format::eR8G8Unorm: return mango::Format::R8G8Unorm;
		case vk::Format::eR8G8Snorm: return mango::Format::R8G8Snorm;
		case vk::Format::eR8G8Uscaled: return mango::Format::R8G8Uscaled;
		case vk::Format::eR8G8Sscaled: return mango::Format::R8G8Sscaled;
		case vk::Format::eR8G8Uint: return mango::Format::R8G8Uint;
		case vk::Format::eR8G8Sint: return mango::Format::R8G8Sint;
		case vk::Format::eR8G8Srgb: return mango::Format::R8G8Srgb;
		case vk::Format::eR8G8B8Unorm: return mango::Format::R8G8B8Unorm;
		case vk::Format::eR8G8B8Snorm: return mango::Format::R8G8B8Snorm;
		case vk::Format::eR8G8B8Uscaled: return mango::Format::R8G8B8Uscaled;
		case vk::Format::eR8G8B8Sscaled: return mango::Format::R8G8B8Sscaled;
		case vk::Format::eR8G8B8Uint: return mango::Format::R8G8B8Uint;
		case vk::Format::eR8G8B8Sint: return mango::Format::R8G8B8Sint;
		case vk::Format::eR8G8B8Srgb: return mango::Format::R8G8B8Srgb;
		case vk::Format::eB8G8R8Unorm: return mango::Format::B8G8R8Unorm;
		case vk::Format::eB8G8R8Snorm: return mango::Format::B8G8R8Snorm;
		case vk::Format::eB8G8R8Uscaled: return mango::Format::B8G8R8Uscaled;
		case vk::Format::eB8G8R8Sscaled: return mango::Format::B8G8R8Sscaled;
		case vk::Format::eB8G8R8Uint: return mango::Format::B8G8R8Uint;
		case vk::Format::eB8G8R8Sint: return mango::Format::B8G8R8Sint;
		case vk::Format::eB8G8R8Srgb: return mango::Format::B8G8R8Srgb;
		case vk::Format::eR8G8B8A8Unorm: return mango::Format::R8G8B8A8Unorm;
		case vk::Format::eR8G8B8A8Snorm: return mango::Format::R8G8B8A8Snorm;
		case vk::Format::eR8G8B8A8Uscaled: return mango::Format::R8G8B8A8Uscaled;
		case vk::Format::eR8G8B8A8Sscaled: return mango::Format::R8G8B8A8Sscaled;
		case vk::Format::eR8G8B8A8Uint: return mango::Format::R8G8B8A8Uint;
		case vk::Format::eR8G8B8A8Sint: return mango::Format::R8G8B8A8Sint;
		case vk::Format::eR8G8B8A8Srgb: return mango::Format::R8G8B8A8Srgb;
		case vk::Format::eB8G8R8A8Unorm: return mango::Format::B8G8R8A8Unorm;
		case vk::Format::eB8G8R8A8Snorm: return mango::Format::B8G8R8A8Snorm;
		case vk::Format::eB8G8R8A8Uscaled: return mango::Format::B8G8R8A8Uscaled;
		case vk::Format::eB8G8R8A8Sscaled: return mango::Format::B8G8R8A8Sscaled;
		case vk::Format::eB8G8R8A8Uint: return mango::Format::B8G8R8A8Uint;
		case vk::Format::eB8G8R8A8Sint: return mango::Format::B8G8R8A8Sint;
		case vk::Format::eB8G8R8A8Srgb: return mango::Format::B8G8R8A8Srgb;
		case vk::Format::eA8B8G8R8UnormPack32: return mango::Format::A8B8G8R8UnormPack32;
		case vk::Format::eA8B8G8R8SnormPack32: return mango::Format::A8B8G8R8SnormPack32;
		case vk::Format::eA8B8G8R8UscaledPack32: return mango::Format::A8B8G8R8UscaledPack32;
		case vk::Format::eA8B8G8R8SscaledPack32: return mango::Format::A8B8G8R8SscaledPack32;
		case vk::Format::eA8B8G8R8UintPack32: return mango::Format::A8B8G8R8UintPack32;
		case vk::Format::eA8B8G8R8SintPack32: return mango::Format::A8B8G8R8SintPack32;
		case vk::Format::eA8B8G8R8SrgbPack32: return mango::Format::A8B8G8R8SrgbPack32;
		case vk::Format::eA2R10G10B10UnormPack32: return mango::Format::A2R10G10B10UnormPack32;
		case vk::Format::eA2R10G10B10SnormPack32: return mango::Format::A2R10G10B10SnormPack32;
		case vk::Format::eA2R10G10B10UscaledPack32: return mango::Format::A2R10G10B10UscaledPack32;
		case vk::Format::eA2R10G10B10SscaledPack32: return mango::Format::A2R10G10B10SscaledPack32;
		case vk::Format::eA2R10G10B10UintPack32: return mango::Format::A2R10G10B10UintPack32;
		case vk::Format::eA2R10G10B10SintPack32: return mango::Format::A2R10G10B10SintPack32;
		case vk::Format::eA2B10G10R10UnormPack32: return mango::Format::A2B10G10R10UnormPack32;
		case vk::Format::eA2B10G10R10SnormPack32: return mango::Format::A2B10G10R10SnormPack32;
		case vk::Format::eA2B10G10R10UscaledPack32: return mango::Format::A2B10G10R10UscaledPack32;
		case vk::Format::eA2B10G10R10SscaledPack32: return mango::Format::A2B10G10R10SscaledPack32;
		case vk::Format::eA2B10G10R10UintPack32: return mango::Format::A2B10G10R10UintPack32;
		case vk::Format::eA2B10G10R10SintPack32: return mango::Format::A2B10G10R10SintPack32;
		case vk::Format::eR16Unorm: return mango::Format::R16Unorm;
		case vk::Format::eR16Snorm: return mango::Format::R16Snorm;
		case vk::Format::eR16Uscaled: return mango::Format::R16Uscaled;
		case vk::Format::eR16Sscaled: return mango::Format::R16Sscaled;
		case vk::Format::eR16Uint: return mango::Format::R16Uint;
		case vk::Format::eR16Sint: return mango::Format::R16Sint;
		case vk::Format::eR16Sfloat: return mango::Format::R16Sfloat;
		case vk::Format::eR16G16Unorm: return mango::Format::R16G16Unorm;
		case vk::Format::eR16G16Snorm: return mango::Format::R16G16Snorm;
		case vk::Format::eR16G16Uscaled: return mango::Format::R16G16Uscaled;
		case vk::Format::eR16G16Sscaled: return mango::Format::R16G16Sscaled;
		case vk::Format::eR16G16Uint: return mango::Format::R16G16Uint;
		case vk::Format::eR16G16Sint: return mango::Format::R16G16Sint;
		case vk::Format::eR16G16Sfloat: return mango::Format::R16G16Sfloat;
		case vk::Format::eR16G16B16Unorm: return mango::Format::R16G16B16Unorm;
		case vk::Format::eR16G16B16Snorm: return mango::Format::R16G16B16Snorm;
		case vk::Format::eR16G16B16Uscaled: return mango::Format::R16G16B16Uscaled;
		case vk::Format::eR16G16B16Sscaled: return mango::Format::R16G16B16Sscaled;
		case vk::Format::eR16G16B16Uint: return mango::Format::R16G16B16Uint;
		case vk::Format::eR16G16B16Sint: return mango::Format::R16G16B16Sint;
		case vk::Format::eR16G16B16Sfloat: return mango::Format::R16G16B16Sfloat;
		case vk::Format::eR16G16B16A16Unorm: return mango::Format::R16G16B16A16Unorm;
		case vk::Format::eR16G16B16A16Snorm: return mango::Format::R16G16B16A16Snorm;
		case vk::Format::eR16G16B16A16Uscaled: return mango::Format::R16G16B16A16Uscaled;
		case vk::Format::eR16G16B16A16Sscaled: return mango::Format::R16G16B16A16Sscaled;
		case vk::Format::eR16G16B16A16Uint: return mango::Format::R16G16B16A16Uint;
		case vk::Format::eR16G16B16A16Sint: return mango::Format::R16G16B16A16Sint;
		case vk::Format::eR16G16B16A16Sfloat: return mango::Format::R16G16B16A16Sfloat;
		case vk::Format::eR32Uint: return mango::Format::R32Uint;
		case vk::Format::eR32Sint: return mango::Format::R32Sint;
		case vk::Format::eR32Sfloat: return mango::Format::R32Sfloat;
		case vk::Format::eR32G32Uint: return mango::Format::R32G32Uint;
		case vk::Format::eR32G32Sint: return mango::Format::R32G32Sint;
		case vk::Format::eR32G32Sfloat: return mango::Format::R32G32Sfloat;
		case vk::Format::eR32G32B32Uint: return mango::Format::R32G32B32Uint;
		case vk::Format::eR32G32B32Sint: return mango::Format::R32G32B32Sint;
		case vk::Format::eR32G32B32Sfloat: return mango::Format::R32G32B32Sfloat;
		case vk::Format::eR32G32B32A32Uint: return mango::Format::R32G32B32A32Uint;
		case vk::Format::eR32G32B32A32Sint: return mango::Format::R32G32B32A32Sint;
		case vk::Format::eR32G32B32A32Sfloat: return mango::Format::R32G32B32A32Sfloat;
		case vk::Format::eR64Uint: return mango::Format::R64Uint;
		case vk::Format::eR64Sint: return mango::Format::R64Sint;
		case vk::Format::eR64Sfloat: return mango::Format::R64Sfloat;
		case vk::Format::eR64G64Uint: return mango::Format::R64G64Uint;
		case vk::Format::eR64G64Sint: return mango::Format::R64G64Sint;
		case vk::Format::eR64G64Sfloat: return mango::Format::R64G64Sfloat;
		case vk::Format::eR64G64B64Uint: return mango::Format::R64G64B64Uint;
		case vk::Format::eR64G64B64Sint: return mango::Format::R64G64B64Sint;
		case vk::Format::eR64G64B64Sfloat: return mango::Format::R64G64B64Sfloat;
		case vk::Format::eR64G64B64A64Uint: return mango::Format::R64G64B64A64Uint;
		case vk::Format::eR64G64B64A64Sint: return mango::Format::R64G64B64A64Sint;
		case vk::Format::eR64G64B64A64Sfloat: return mango::Format::R64G64B64A64Sfloat;
		case vk::Format::eB10G11R11UfloatPack32: return mango::Format::B10G11R11UfloatPack32;
		case vk::Format::eE5B9G9R9UfloatPack32: return mango::Format::E5B9G9R9UfloatPack32;
		case vk::Format::eD16Unorm: return mango::Format::D16Unorm;
		case vk::Format::eX8D24UnormPack32: return mango::Format::X8D24UnormPack32;
		case vk::Format::eD32Sfloat: return mango::Format::D32Sfloat;
		case vk::Format::eS8Uint: return mango::Format::S8Uint;
		case vk::Format::eD16UnormS8Uint: return mango::Format::D16UnormS8Uint;
		case vk::Format::eD24UnormS8Uint: return mango::Format::D24UnormS8Uint;
		case vk::Format::eD32SfloatS8Uint: return mango::Format::D32SfloatS8Uint;
		case vk::Format::eBc1RgbUnormBlock: return mango::Format::Bc1RgbUnormBlock;
		case vk::Format::eBc1RgbSrgbBlock: return mango::Format::Bc1RgbSrgbBlock;
		case vk::Format::eBc1RgbaUnormBlock: return mango::Format::Bc1RgbaUnormBlock;
		case vk::Format::eBc1RgbaSrgbBlock: return mango::Format::Bc1RgbaSrgbBlock;
		case vk::Format::eBc2UnormBlock: return mango::Format::Bc2UnormBlock;
		case vk::Format::eBc2SrgbBlock: return mango::Format::Bc2SrgbBlock;
		case vk::Format::eBc3UnormBlock: return mango::Format::Bc3UnormBlock;
		case vk::Format::eBc3SrgbBlock: return mango::Format::Bc3SrgbBlock;
		case vk::Format::eBc4UnormBlock: return mango::Format::Bc4UnormBlock;
		case vk::Format::eBc4SnormBlock: return mango::Format::Bc4SnormBlock;
		case vk::Format::eBc5UnormBlock: return mango::Format::Bc5UnormBlock;
		case vk::Format::eBc5SnormBlock: return mango::Format::Bc5SnormBlock;
		case vk::Format::eBc6HUfloatBlock: return mango::Format::Bc6HUfloatBlock;
		case vk::Format::eBc6HSfloatBlock: return mango::Format::Bc6HSfloatBlock;
		case vk::Format::eBc7UnormBlock: return mango::Format::Bc7UnormBlock;
		case vk::Format::eBc7SrgbBlock: return mango::Format::Bc7SrgbBlock;
		case vk::Format::eEtc2R8G8B8UnormBlock: return mango::Format::Etc2R8G8B8UnormBlock;
		case vk::Format::eEtc2R8G8B8SrgbBlock: return mango::Format::Etc2R8G8B8SrgbBlock;
		case vk::Format::eEtc2R8G8B8A1UnormBlock: return mango::Format::Etc2R8G8B8A1UnormBlock;
		case vk::Format::eEtc2R8G8B8A1SrgbBlock: return mango::Format::Etc2R8G8B8A1SrgbBlock;
		case vk::Format::eEtc2R8G8B8A8UnormBlock: return mango::Format::Etc2R8G8B8A8UnormBlock;
		case vk::Format::eEtc2R8G8B8A8SrgbBlock: return mango::Format::Etc2R8G8B8A8SrgbBlock;
		case vk::Format::eEacR11UnormBlock: return mango::Format::EacR11UnormBlock;
		case vk::Format::eEacR11SnormBlock: return mango::Format::EacR11SnormBlock;
		case vk::Format::eEacR11G11UnormBlock: return mango::Format::EacR11G11UnormBlock;
		case vk::Format::eEacR11G11SnormBlock: return mango::Format::EacR11G11SnormBlock;
		case vk::Format::eAstc4x4UnormBlock: return mango::Format::Astc4x4UnormBlock;
		case vk::Format::eAstc4x4SrgbBlock: return mango::Format::Astc4x4SrgbBlock;
		case vk::Format::eAstc5x4UnormBlock: return mango::Format::Astc5x4UnormBlock;
		case vk::Format::eAstc5x4SrgbBlock: return mango::Format::Astc5x4SrgbBlock;
		case vk::Format::eAstc5x5UnormBlock: return mango::Format::Astc5x5UnormBlock;
		case vk::Format::eAstc5x5SrgbBlock: return mango::Format::Astc5x5SrgbBlock;
		case vk::Format::eAstc6x5UnormBlock: return mango::Format::Astc6x5UnormBlock;
		case vk::Format::eAstc6x5SrgbBlock: return mango::Format::Astc6x5SrgbBlock;
		case vk::Format::eAstc6x6UnormBlock: return mango::Format::Astc6x6UnormBlock;
		case vk::Format::eAstc6x6SrgbBlock: return mango::Format::Astc6x6SrgbBlock;
		case vk::Format::eAstc8x5UnormBlock: return mango::Format::Astc8x5UnormBlock;
		case vk::Format::eAstc8x5SrgbBlock: return mango::Format::Astc8x5SrgbBlock;
		case vk::Format::eAstc8x6UnormBlock: return mango::Format::Astc8x6UnormBlock;
		case vk::Format::eAstc8x6SrgbBlock: return mango::Format::Astc8x6SrgbBlock;
		case vk::Format::eAstc8x8UnormBlock: return mango::Format::Astc8x8UnormBlock;
		case vk::Format::eAstc8x8SrgbBlock: return mango::Format::Astc8x8SrgbBlock;
		case vk::Format::eAstc10x5UnormBlock: return mango::Format::Astc10x5UnormBlock;
		case vk::Format::eAstc10x5SrgbBlock: return mango::Format::Astc10x5SrgbBlock;
		case vk::Format::eAstc10x6UnormBlock: return mango::Format::Astc10x6UnormBlock;
		case vk::Format::eAstc10x6SrgbBlock: return mango::Format::Astc10x6SrgbBlock;
		case vk::Format::eAstc10x8UnormBlock: return mango::Format::Astc10x8UnormBlock;
		case vk::Format::eAstc10x8SrgbBlock: return mango::Format::Astc10x8SrgbBlock;
		case vk::Format::eAstc10x10UnormBlock: return mango::Format::Astc10x10UnormBlock;
		case vk::Format::eAstc10x10SrgbBlock: return mango::Format::Astc10x10SrgbBlock;
		case vk::Format::eAstc12x10UnormBlock: return mango::Format::Astc12x10UnormBlock;
		case vk::Format::eAstc12x10SrgbBlock: return mango::Format::Astc12x10SrgbBlock;
		case vk::Format::eAstc12x12UnormBlock: return mango::Format::Astc12x12UnormBlock;
		case vk::Format::eAstc12x12SrgbBlock: return mango::Format::Astc12x12SrgbBlock;
		case vk::Format::eG8B8G8R8422Unorm: return mango::Format::G8B8G8R8422Unorm;
		case vk::Format::eB8G8R8G8422Unorm: return mango::Format::B8G8R8G8422Unorm;
		case vk::Format::eG8B8R83Plane420Unorm: return mango::Format::G8B8R83Plane420Unorm;
		case vk::Format::eG8B8R82Plane420Unorm: return mango::Format::G8B8R82Plane420Unorm;
		case vk::Format::eG8B8R83Plane422Unorm: return mango::Format::G8B8R83Plane422Unorm;
		case vk::Format::eG8B8R82Plane422Unorm: return mango::Format::G8B8R82Plane422Unorm;
		case vk::Format::eG8B8R83Plane444Unorm: return mango::Format::G8B8R83Plane444Unorm;
		case vk::Format::eR10X6UnormPack16: return mango::Format::R10X6UnormPack16;
		case vk::Format::eR10X6G10X6Unorm2Pack16: return mango::Format::R10X6G10X6Unorm2Pack16;
		case vk::Format::eR10X6G10X6B10X6A10X6Unorm4Pack16: return mango::Format::R10X6G10X6B10X6A10X6Unorm4Pack16;
		case vk::Format::eG10X6B10X6G10X6R10X6422Unorm4Pack16: return mango::Format::G10X6B10X6G10X6R10X6422Unorm4Pack16;
		case vk::Format::eB10X6G10X6R10X6G10X6422Unorm4Pack16: return mango::Format::B10X6G10X6R10X6G10X6422Unorm4Pack16;
		case vk::Format::eG10X6B10X6R10X63Plane420Unorm3Pack16: return mango::Format::G10X6B10X6R10X63Plane420Unorm3Pack16;
		case vk::Format::eG10X6B10X6R10X62Plane420Unorm3Pack16: return mango::Format::G10X6B10X6R10X62Plane420Unorm3Pack16;
		case vk::Format::eG10X6B10X6R10X63Plane422Unorm3Pack16: return mango::Format::G10X6B10X6R10X63Plane422Unorm3Pack16;
		case vk::Format::eG10X6B10X6R10X62Plane422Unorm3Pack16: return mango::Format::G10X6B10X6R10X62Plane422Unorm3Pack16;
		case vk::Format::eG10X6B10X6R10X63Plane444Unorm3Pack16: return mango::Format::G10X6B10X6R10X63Plane444Unorm3Pack16;
		case vk::Format::eR12X4UnormPack16: return mango::Format::R12X4UnormPack16;
		case vk::Format::eR12X4G12X4Unorm2Pack16: return mango::Format::R12X4G12X4Unorm2Pack16;
		case vk::Format::eR12X4G12X4B12X4A12X4Unorm4Pack16: return mango::Format::R12X4G12X4B12X4A12X4Unorm4Pack16;
		case vk::Format::eG12X4B12X4G12X4R12X4422Unorm4Pack16: return mango::Format::G12X4B12X4G12X4R12X4422Unorm4Pack16;
		case vk::Format::eB12X4G12X4R12X4G12X4422Unorm4Pack16: return mango::Format::B12X4G12X4R12X4G12X4422Unorm4Pack16;
		case vk::Format::eG12X4B12X4R12X43Plane420Unorm3Pack16: return mango::Format::G12X4B12X4R12X43Plane420Unorm3Pack16;
		case vk::Format::eG12X4B12X4R12X42Plane420Unorm3Pack16: return mango::Format::G12X4B12X4R12X42Plane420Unorm3Pack16;
		case vk::Format::eG12X4B12X4R12X43Plane422Unorm3Pack16: return mango::Format::G12X4B12X4R12X43Plane422Unorm3Pack16;
		case vk::Format::eG12X4B12X4R12X42Plane422Unorm3Pack16: return mango::Format::G12X4B12X4R12X42Plane422Unorm3Pack16;
		case vk::Format::eG12X4B12X4R12X43Plane444Unorm3Pack16: return mango::Format::G12X4B12X4R12X43Plane444Unorm3Pack16;
		case vk::Format::eG16B16G16R16422Unorm: return mango::Format::G16B16G16R16422Unorm;
		case vk::Format::eB16G16R16G16422Unorm: return mango::Format::B16G16R16G16422Unorm;
		case vk::Format::eG16B16R163Plane420Unorm: return mango::Format::G16B16R163Plane420Unorm;
		case vk::Format::eG16B16R162Plane420Unorm: return mango::Format::G16B16R162Plane420Unorm;
		case vk::Format::eG16B16R163Plane422Unorm: return mango::Format::G16B16R163Plane422Unorm;
		case vk::Format::eG16B16R162Plane422Unorm: return mango::Format::G16B16R162Plane422Unorm;
		case vk::Format::eG16B16R163Plane444Unorm: return mango::Format::G16B16R163Plane444Unorm;
		case vk::Format::ePvrtc12BppUnormBlockIMG: return mango::Format::Pvrtc12BppUnormBlockIMG;
		case vk::Format::ePvrtc14BppUnormBlockIMG: return mango::Format::Pvrtc14BppUnormBlockIMG;
		case vk::Format::ePvrtc22BppUnormBlockIMG: return mango::Format::Pvrtc22BppUnormBlockIMG;
		case vk::Format::ePvrtc24BppUnormBlockIMG: return mango::Format::Pvrtc24BppUnormBlockIMG;
		case vk::Format::ePvrtc12BppSrgbBlockIMG: return mango::Format::Pvrtc12BppSrgbBlockIMG;
		case vk::Format::ePvrtc14BppSrgbBlockIMG: return mango::Format::Pvrtc14BppSrgbBlockIMG;
		case vk::Format::ePvrtc22BppSrgbBlockIMG: return mango::Format::Pvrtc22BppSrgbBlockIMG;
		case vk::Format::ePvrtc24BppSrgbBlockIMG: return mango::Format::Pvrtc24BppSrgbBlockIMG;
	}
}

vk::ComponentSwizzle mango::vulkan::componentSwizzleVK(const mango::ComponentSwizzle& _mango){
	switch( _mango ) {
		case mango::ComponentSwizzle::Identity:
		return vk::ComponentSwizzle::eIdentity;
		case mango::ComponentSwizzle::One:
		return vk::ComponentSwizzle::eOne;
		case mango::ComponentSwizzle::Zero:
		return vk::ComponentSwizzle::eZero;
		case mango::ComponentSwizzle::R:
		return vk::ComponentSwizzle::eR;
		case mango::ComponentSwizzle::G:
		return vk::ComponentSwizzle::eG;
		case mango::ComponentSwizzle::B:
		return vk::ComponentSwizzle::eB;
		case mango::ComponentSwizzle::A:
		return vk::ComponentSwizzle::eA;
	}
}

vk::ComponentMapping mango::vulkan::componentMappingVK(const mango::ComponentMapping& _mango){
	return vk::ComponentMapping(
		mango::vulkan::componentSwizzleVK(_mango.R),
		mango::vulkan::componentSwizzleVK(_mango.G),
		mango::vulkan::componentSwizzleVK(_mango.B),
		mango::vulkan::componentSwizzleVK(_mango.A));
}

vk::ShaderStageFlagBits mango::vulkan::shaderStageVK(const mango::ShaderStage& _mango){
	switch(_mango){
		case mango::ShaderStage::Vertex: return vk::ShaderStageFlagBits::eVertex;
		case mango::ShaderStage::TessellationControl: return vk::ShaderStageFlagBits::eTessellationControl;
		case mango::ShaderStage::TessellationEvaluation: return vk::ShaderStageFlagBits::eTessellationEvaluation;
		case mango::ShaderStage::Geometry: return vk::ShaderStageFlagBits::eGeometry;
		case mango::ShaderStage::Fragment: return vk::ShaderStageFlagBits::eFragment;
		case mango::ShaderStage::Compute: return vk::ShaderStageFlagBits::eCompute;
		case mango::ShaderStage::AllGraphics: return vk::ShaderStageFlagBits::eAllGraphics;
		case mango::ShaderStage::All: return vk::ShaderStageFlagBits::eAll;
	}
}
