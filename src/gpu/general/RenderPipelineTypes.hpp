//
// Created by kurono267 on 30.06.2021.
//

#ifndef MANGORENDERER_RENDERPIPELINETYPES_HPP
#define MANGORENDERER_RENDERPIPELINETYPES_HPP

#include "third_party.hpp"
#include <vector>

enum class PrimitiveTopology {
		PointList,
		LineList,
		LineStrip,
		TriangleList,
		TriangleStrip,
		TriangleFan,
		LineListWithAdjacency,
		LineStripWithAdjacency,
		TriangleListWithAdjacency,
		TriangleStripWithAdjacency,
		PatchList
};

struct Viewport {
	Viewport(const glm::vec2& _start,const glm::vec2& _size,const float _minDepth = 0.0f,float _maxDepth = 1.0f) :
			start(_start), size(_size), minDepth(_minDepth), maxDepth(_maxDepth) {}
	Viewport() = default;

	glm::vec2 start;
	glm::vec2 size;
	float minDepth;
	float maxDepth;
};

enum class PolygonMode {
		Fill, Line, Point
};

enum class CullMode {
		None, Front, Back, FrontAndBack
};

enum class FrontFace {
		CCW, CW
};

enum class ColorComponent {
		eR = 0x0001, eG = 0x0002, eB = 0x0004, eA = 0x0008
};

const uint16_t ColorRGBA = (uint16_t)ColorComponent::eR | (uint32_t)ColorComponent::eG | (uint32_t)ColorComponent::eB | (uint32_t)ColorComponent::eA;
const uint16_t ColorRGB = (uint16_t)ColorComponent::eR | (uint32_t)ColorComponent::eG | (uint32_t)ColorComponent::eB;

enum class BlendFactor {
		Zero,
		One,
		SrcColor,
		OneMinusSrcColor,
		DstColor,
		OneMinusDstColor,
		SrcAlpha,
		OneMinusSrcAlpha,
		DstAlpha,
		OneMinusDstAlpha,
		ConstantColor,
		OneMinusConstantColor,
		ConstantAlpha,
		OneMinusConstantAlpha,
		SrcAlphaSaturate,
		Src1Color,
		OneMinusSrc1Color,
		Src1Alpha,
		OneMinusSrc1Alpha
};

enum class CompareOp {
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
};

enum class BlendOp {
		Add,
		Subtract,
		ReverseSubtract,
		Min,
		Max,
		ZeroEXT,
		SrcEXT,
		DstEXT,
		SrcOverEXT,
		DstOverEXT,
		SrcInEXT,
		DstInEXT,
		SrcOutEXT,
		DstOutEXT,
		SrcAtopEXT,
		DstAtopEXT,
		XorEXT,
		MultiplyEXT,
		ScreenEXT,
		OverlayEXT,
		DarkenEXT,
		LightenEXT,
		ColordodgeEXT,
		ColorburnEXT,
		HardlightEXT,
		SoftlightEXT,
		DifferenceEXT,
		ExclusionEXT,
		InvertEXT,
		InvertRgbEXT,
		LineardodgeEXT,
		LinearburnEXT,
		VividlightEXT,
		LinearlightEXT,
		PinlightEXT,
		HardmixEXT,
		HslHueEXT,
		HslSaturationEXT,
		HslColorEXT,
		HslLuminosityEXT,
		PlusEXT,
		PlusClampedEXT,
		PlusClampedAlphaEXT,
		PlusDarkerEXT,
		MinusEXT,
		MinusClampedEXT,
		ContrastEXT,
		InvertOvgEXT,
		RedEXT,
		GreenEXT,
		BlueEXT
};

enum class ShaderStage {
		Vertex,
		TessellationControl,
		TessellationEvaluation,
		Geometry,
		Fragment,
		Compute,
		AllGraphics,
		All
};

struct RasterizationState {
	PolygonMode polyMode;
	CullMode    cullMode;
	FrontFace    face;

	float lineWidth;
	bool  depthClamp;
	bool  discard;
	bool  depthBias;
	glm::vec3 depthBiasFactor;
};

struct BlendState {
	bool enable = true;
	uint16_t writeMask = ColorRGBA;
	BlendFactor srcColorBlendFactor = BlendFactor::SrcAlpha;
	BlendFactor dstColorBlendFactor = BlendFactor::OneMinusSrcAlpha;
	BlendOp colorBlendOp = BlendOp::Add;
	BlendFactor srcAlphaBlendFactor = BlendFactor::One;
	BlendFactor dstAlphaBlendFactor = BlendFactor::Zero;
	BlendOp alphaBlendOp = BlendOp::Add;
};

struct DepthState {
	bool enable;
	bool write;
	CompareOp comp;
};

#endif //MANGORENDERER_RENDERPIPELINETYPES_HPP
