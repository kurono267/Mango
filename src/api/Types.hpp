//
// Created by kurono267 on 05.06.18.
//

#ifndef MANGO_TYPES_HPP
#define MANGO_TYPES_HPP

#include "default.hpp"

namespace mango {

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
	Viewport(){}

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

enum class SampleCount {
	s1, s2, s4, s8, s16, s32, s64
};

enum class ColorComponent {
	eR, eG, eB, eA
};

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

enum class Format {
	Undefined,
	R4G4UnormPack8,
	R4G4B4A4UnormPack16,
	B4G4R4A4UnormPack16,
	R5G6B5UnormPack16,
	B5G6R5UnormPack16,
	R5G5B5A1UnormPack16,
	B5G5R5A1UnormPack16,
	A1R5G5B5UnormPack16,
	R8Unorm,
	R8Snorm,
	R8Uscaled,
	R8Sscaled,
	R8Uint,
	R8Sint,
	R8Srgb,
	R8G8Unorm,
	R8G8Snorm,
	R8G8Uscaled,
	R8G8Sscaled,
	R8G8Uint,
	R8G8Sint,
	R8G8Srgb,
	R8G8B8Unorm,
	R8G8B8Snorm,
	R8G8B8Uscaled,
	R8G8B8Sscaled,
	R8G8B8Uint,
	R8G8B8Sint,
	R8G8B8Srgb,
	B8G8R8Unorm,
	B8G8R8Snorm,
	B8G8R8Uscaled,
	B8G8R8Sscaled,
	B8G8R8Uint,
	B8G8R8Sint,
	B8G8R8Srgb,
	R8G8B8A8Unorm,
	R8G8B8A8Snorm,
	R8G8B8A8Uscaled,
	R8G8B8A8Sscaled,
	R8G8B8A8Uint,
	R8G8B8A8Sint,
	R8G8B8A8Srgb,
	B8G8R8A8Unorm,
	B8G8R8A8Snorm,
	B8G8R8A8Uscaled,
	B8G8R8A8Sscaled,
	B8G8R8A8Uint,
	B8G8R8A8Sint,
	B8G8R8A8Srgb,
	A8B8G8R8UnormPack32,
	A8B8G8R8SnormPack32,
	A8B8G8R8UscaledPack32,
	A8B8G8R8SscaledPack32,
	A8B8G8R8UintPack32,
	A8B8G8R8SintPack32,
	A8B8G8R8SrgbPack32,
	A2R10G10B10UnormPack32,
	A2R10G10B10SnormPack32,
	A2R10G10B10UscaledPack32,
	A2R10G10B10SscaledPack32,
	A2R10G10B10UintPack32,
	A2R10G10B10SintPack32,
	A2B10G10R10UnormPack32,
	A2B10G10R10SnormPack32,
	A2B10G10R10UscaledPack32,
	A2B10G10R10SscaledPack32,
	A2B10G10R10UintPack32,
	A2B10G10R10SintPack32,
	R16Unorm,
	R16Snorm,
	R16Uscaled,
	R16Sscaled,
	R16Uint,
	R16Sint,
	R16Sfloat,
	R16G16Unorm,
	R16G16Snorm,
	R16G16Uscaled,
	R16G16Sscaled,
	R16G16Uint,
	R16G16Sint,
	R16G16Sfloat,
	R16G16B16Unorm,
	R16G16B16Snorm,
	R16G16B16Uscaled,
	R16G16B16Sscaled,
	R16G16B16Uint,
	R16G16B16Sint,
	R16G16B16Sfloat,
	R16G16B16A16Unorm,
	R16G16B16A16Snorm,
	R16G16B16A16Uscaled,
	R16G16B16A16Sscaled,
	R16G16B16A16Uint,
	R16G16B16A16Sint,
	R16G16B16A16Sfloat,
	R32Uint,
	R32Sint,
	R32Sfloat,
	R32G32Uint,
	R32G32Sint,
	R32G32Sfloat,
	R32G32B32Uint,
	R32G32B32Sint,
	R32G32B32Sfloat,
	R32G32B32A32Uint,
	R32G32B32A32Sint,
	R32G32B32A32Sfloat,
	R64Uint,
	R64Sint,
	R64Sfloat,
	R64G64Uint,
	R64G64Sint,
	R64G64Sfloat,
	R64G64B64Uint,
	R64G64B64Sint,
	R64G64B64Sfloat,
	R64G64B64A64Uint,
	R64G64B64A64Sint,
	R64G64B64A64Sfloat,
	B10G11R11UfloatPack32,
	E5B9G9R9UfloatPack32,
	D16Unorm,
	X8D24UnormPack32,
	D32Sfloat,
	S8Uint,
	D16UnormS8Uint,
	D24UnormS8Uint,
	D32SfloatS8Uint,
	Bc1RgbUnormBlock,
	Bc1RgbSrgbBlock,
	Bc1RgbaUnormBlock,
	Bc1RgbaSrgbBlock,
	Bc2UnormBlock,
	Bc2SrgbBlock,
	Bc3UnormBlock,
	Bc3SrgbBlock,
	Bc4UnormBlock,
	Bc4SnormBlock,
	Bc5UnormBlock,
	Bc5SnormBlock,
	Bc6HUfloatBlock,
	Bc6HSfloatBlock,
	Bc7UnormBlock,
	Bc7SrgbBlock,
	Etc2R8G8B8UnormBlock,
	Etc2R8G8B8SrgbBlock,
	Etc2R8G8B8A1UnormBlock,
	Etc2R8G8B8A1SrgbBlock,
	Etc2R8G8B8A8UnormBlock,
	Etc2R8G8B8A8SrgbBlock,
	EacR11UnormBlock,
	EacR11SnormBlock,
	EacR11G11UnormBlock,
	EacR11G11SnormBlock,
	Astc4x4UnormBlock,
	Astc4x4SrgbBlock,
	Astc5x4UnormBlock,
	Astc5x4SrgbBlock,
	Astc5x5UnormBlock,
	Astc5x5SrgbBlock,
	Astc6x5UnormBlock,
	Astc6x5SrgbBlock,
	Astc6x6UnormBlock,
	Astc6x6SrgbBlock,
	Astc8x5UnormBlock,
	Astc8x5SrgbBlock,
	Astc8x6UnormBlock,
	Astc8x6SrgbBlock,
	Astc8x8UnormBlock,
	Astc8x8SrgbBlock,
	Astc10x5UnormBlock,
	Astc10x5SrgbBlock,
	Astc10x6UnormBlock,
	Astc10x6SrgbBlock,
	Astc10x8UnormBlock,
	Astc10x8SrgbBlock,
	Astc10x10UnormBlock,
	Astc10x10SrgbBlock,
	Astc12x10UnormBlock,
	Astc12x10SrgbBlock,
	Astc12x12UnormBlock,
	Astc12x12SrgbBlock,
	G8B8G8R8422Unorm,
	G8B8G8R8422UnormKHR,
	B8G8R8G8422Unorm,
	B8G8R8G8422UnormKHR,
	G8B8R83Plane420Unorm,
	G8B8R83Plane420UnormKHR,
	G8B8R82Plane420Unorm,
	G8B8R82Plane420UnormKHR,
	G8B8R83Plane422Unorm,
	G8B8R83Plane422UnormKHR,
	G8B8R82Plane422Unorm,
	G8B8R82Plane422UnormKHR,
	G8B8R83Plane444Unorm,
	G8B8R83Plane444UnormKHR,
	R10X6UnormPack16,
	R10X6UnormPack16KHR,
	R10X6G10X6Unorm2Pack16,
	R10X6G10X6Unorm2Pack16KHR,
	R10X6G10X6B10X6A10X6Unorm4Pack16,
	R10X6G10X6B10X6A10X6Unorm4Pack16KHR,
	G10X6B10X6G10X6R10X6422Unorm4Pack16,
	G10X6B10X6G10X6R10X6422Unorm4Pack16KHR,
	B10X6G10X6R10X6G10X6422Unorm4Pack16,
	B10X6G10X6R10X6G10X6422Unorm4Pack16KHR,
	G10X6B10X6R10X63Plane420Unorm3Pack16,
	G10X6B10X6R10X63Plane420Unorm3Pack16KHR,
	G10X6B10X6R10X62Plane420Unorm3Pack16,
	G10X6B10X6R10X62Plane420Unorm3Pack16KHR,
	G10X6B10X6R10X63Plane422Unorm3Pack16,
	G10X6B10X6R10X63Plane422Unorm3Pack16KHR,
	G10X6B10X6R10X62Plane422Unorm3Pack16,
	G10X6B10X6R10X62Plane422Unorm3Pack16KHR,
	G10X6B10X6R10X63Plane444Unorm3Pack16,
	G10X6B10X6R10X63Plane444Unorm3Pack16KHR,
	R12X4UnormPack16,
	R12X4UnormPack16KHR,
	R12X4G12X4Unorm2Pack16,
	R12X4G12X4Unorm2Pack16KHR,
	R12X4G12X4B12X4A12X4Unorm4Pack16,
	R12X4G12X4B12X4A12X4Unorm4Pack16KHR,
	G12X4B12X4G12X4R12X4422Unorm4Pack16,
	G12X4B12X4G12X4R12X4422Unorm4Pack16KHR,
	B12X4G12X4R12X4G12X4422Unorm4Pack16,
	B12X4G12X4R12X4G12X4422Unorm4Pack16KHR,
	G12X4B12X4R12X43Plane420Unorm3Pack16,
	G12X4B12X4R12X43Plane420Unorm3Pack16KHR,
	G12X4B12X4R12X42Plane420Unorm3Pack16,
	G12X4B12X4R12X42Plane420Unorm3Pack16KHR,
	G12X4B12X4R12X43Plane422Unorm3Pack16,
	G12X4B12X4R12X43Plane422Unorm3Pack16KHR,
	G12X4B12X4R12X42Plane422Unorm3Pack16,
	G12X4B12X4R12X42Plane422Unorm3Pack16KHR,
	G12X4B12X4R12X43Plane444Unorm3Pack16,
	G12X4B12X4R12X43Plane444Unorm3Pack16KHR,
	G16B16G16R16422Unorm,
	G16B16G16R16422UnormKHR,
	B16G16R16G16422Unorm,
	B16G16R16G16422UnormKHR,
	G16B16R163Plane420Unorm,
	G16B16R163Plane420UnormKHR,
	G16B16R162Plane420Unorm,
	G16B16R162Plane420UnormKHR,
	G16B16R163Plane422Unorm,
	G16B16R163Plane422UnormKHR,
	G16B16R162Plane422Unorm,
	G16B16R162Plane422UnormKHR,
	G16B16R163Plane444Unorm,
	G16B16R163Plane444UnormKHR,
	Pvrtc12BppUnormBlockIMG,
	Pvrtc14BppUnormBlockIMG,
	Pvrtc22BppUnormBlockIMG,
	Pvrtc24BppUnormBlockIMG,
	Pvrtc12BppSrgbBlockIMG,
	Pvrtc14BppSrgbBlockIMG,
	Pvrtc22BppSrgbBlockIMG,
	Pvrtc24BppSrgbBlockIMG
};

enum class ComponentSwizzle {
	Identity,
	Zero,
	One,
	R,
	G,
	B,
	A
};

struct ComponentMapping {
	ComponentMapping(const ComponentSwizzle& _r = ComponentSwizzle::Identity,
					 const ComponentSwizzle& _g = ComponentSwizzle::Identity,
					 const ComponentSwizzle& _b = ComponentSwizzle::Identity,
					 const ComponentSwizzle& _a = ComponentSwizzle::Identity) :
		R(_r), G(_g), B(_b), A(_a) {}
	ComponentSwizzle R;
	ComponentSwizzle G;
	ComponentSwizzle B;
	ComponentSwizzle A;
};

enum class BufferType {
	Uniform = 0x01,
	Storage = 0x02,
	Index = 0x04,
	Vertex = 0x08,
	CPU = 0x10
};

BufferType operator |(BufferType lhs, BufferType rhs);

enum class MemoryType {
	HOST,
	DEVICE,
	DEVICE_HOST
};

enum class TextureUsage : unsigned {
	TransferSrc = 1 << 0,
	TransferDst = 1 << 1,
	Sampled = 1 << 2,
	Storage = 1 << 3,
	ColorAttachment = 1 << 4,
	DepthStencilAttachment = 1 << 5,
	TransientAttachment = 1 << 6,
	InputAttachment = 1 << 7
};

TextureUsage operator |(TextureUsage lhs, TextureUsage rhs);

enum class TextureLayout : unsigned {
	Undefined = 0,
	General,
	ColorAttachmentOptimal,
	DepthStencilAttachmentOptimal,
	DepthStencilReadOnlyOptimal,
	ShaderReadOnlyOptimal,
	TransferSrcOptimal,
	TransferDstOptimal,
	Preinitialized,
	DepthReadOnlyStencilAttachmentOptimal,
	DepthAttachmentStencilReadOnlyOptimal,
	DepthAttachmentOptimal,
	DepthReadOnlyOptimal,
	StencilAttachmentOptimal,
	StencilReadOnlyOptimal,
	PresentSrcKHR,
	SharedPresentKHR,
	FragmentDensityMapOptimalEXT,
	DepthAttachmentOptimalKHR,
	DepthAttachmentStencilReadOnlyOptimalKHR,
	DepthReadOnlyOptimalKHR,
	DepthReadOnlyStencilAttachmentOptimalKHR,
	StencilAttachmentOptimalKHR,
	StencilReadOnlyOptimalKHR
};

enum class Filter {
	Nearest,
	Linear,
	Cubic
};

enum class SamplerMipmapMode {
	Nearest,
	Linear
};

enum class SamplerAddressMode {
	Repeat,
	MirroredRepeat,
	ClampToEdge,
	ClampToBorder,
	MirrorClampToEdge
};

enum class BorderColor {
	FloatTransparentBlack,
	IntTransparentBlack,
	FloatOpaqueBlack,
	IntOpaqueBlack,
	FloatOpaqueWhite,
	IntOpaqueWhite
};

enum class PipelineBindPoint {
	Graphics,
	Compute
};

enum class AttachmentStore
{
	Store,
	DontCare
};

enum class AttachmentLoad
{
	Load,
	Clear,
	DontCare
};

};

#endif //MANGO_TYPES_HPP
