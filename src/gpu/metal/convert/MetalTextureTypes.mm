//
// Created by kurono267 on 09.12.2021.
//

#include "MetalTextureTypes.hpp"

MTLPixelFormat formatMetal(const Format &format) {
	switch (format) {
		case Format::Undefined:
			return MTLPixelFormatInvalid;
		case Format::R8Unorm:
			return MTLPixelFormatR8Unorm;
		case Format::R8Snorm:
			return MTLPixelFormatR8Snorm;
		case Format::R8Uint:
			return MTLPixelFormatR8Uint;
		case Format::R8Sint:
			return MTLPixelFormatR8Sint;
		case Format::R8G8B8A8Unorm:
			return MTLPixelFormatRGBA8Unorm;
		case Format::R8G8B8A8Snorm:
			return MTLPixelFormatRGBA8Snorm;
		case Format::R8G8B8A8Uint:
			return MTLPixelFormatRGBA8Uint;
		case Format::R8G8B8A8Sint:
			return MTLPixelFormatRGBA8Sint;
		case Format::R8G8B8A8Srgb:
			return MTLPixelFormatRGBA8Unorm_sRGB;
		case Format::B8G8R8A8Unorm:
			return MTLPixelFormatBGRA8Unorm;
		case Format::B8G8R8A8Srgb:
			return MTLPixelFormatBGRA8Unorm_sRGB;
		case Format::A2R10G10B10UnormPack32:
			return MTLPixelFormatRGB10A2Unorm;
		case Format::A2R10G10B10UintPack32:
			return MTLPixelFormatRGB10A2Uint;
		case Format::A2B10G10R10UnormPack32:
			return MTLPixelFormatBGR10A2Unorm;
		case Format::R16Unorm:
			return MTLPixelFormatR16Unorm;
		case Format::R16Snorm:
			return MTLPixelFormatR16Snorm;
		case Format::R16Uint:
			return MTLPixelFormatR16Uint;
		case Format::R16Sint:
			return MTLPixelFormatR16Sint;
		case Format::R16Sfloat:
			return MTLPixelFormatR16Float;
		case Format::R16G16Unorm:
			return MTLPixelFormatRG16Unorm;
		case Format::R16G16Snorm:
			return MTLPixelFormatRG16Snorm;
		case Format::R16G16Uint:
			return MTLPixelFormatRG16Uint;
		case Format::R16G16Sint:
			return MTLPixelFormatRG16Sint;
		case Format::R16G16Sfloat:
			return MTLPixelFormatRG16Float;
		case Format::R16G16B16A16Unorm:
			return MTLPixelFormatRGBA16Unorm;
		case Format::R16G16B16A16Snorm:
			return MTLPixelFormatRGBA16Snorm;
		case Format::R16G16B16A16Uint:
			return MTLPixelFormatRGBA16Uint;
		case Format::R16G16B16A16Sint:
			return MTLPixelFormatRGBA16Sint;
		case Format::R16G16B16A16Sfloat:
			return MTLPixelFormatRGBA16Float;
		case Format::R32Uint:
			return MTLPixelFormatR32Uint;
		case Format::R32Sint:
			return MTLPixelFormatR32Sint;
		case Format::R32Sfloat:
			return MTLPixelFormatR32Float;
		case Format::R32G32Uint:
			return MTLPixelFormatRG32Uint;
		case Format::R32G32Sint:
			return MTLPixelFormatRG32Sint;
		case Format::R32G32Sfloat:
			return MTLPixelFormatRG32Float;
		case Format::R32G32B32A32Uint:
			return MTLPixelFormatRGBA32Uint;
		case Format::R32G32B32A32Sint:
			return MTLPixelFormatRGBA32Sint;
		case Format::R32G32B32A32Sfloat:
			return MTLPixelFormatRGBA32Float;
		case Format::D16Unorm:
			return MTLPixelFormatDepth16Unorm;
		case Format::D24UnormS8Uint:
			return MTLPixelFormatDepth24Unorm_Stencil8;
		case Format::D32Sfloat:
			return MTLPixelFormatDepth32Float;
		case Format::D32SfloatS8Uint:
			return MTLPixelFormatDepth32Float_Stencil8;
	}
}

Format formatMetal2Mango(const MTLPixelFormat &format) {
	switch (format) {
		case MTLPixelFormatInvalid:
			return Format::Undefined;
		case MTLPixelFormatR8Unorm:
			return Format::R8Unorm;
		case MTLPixelFormatR8Snorm:
			return Format::R8Snorm;
		case MTLPixelFormatR8Uint:
			return Format::R8Uint;
		case MTLPixelFormatR8Sint:
			return Format::R8Sint;
		case MTLPixelFormatRGBA8Unorm:
			return Format::R8G8B8A8Unorm;
		case MTLPixelFormatRGBA8Snorm:
			return Format::R8G8B8A8Snorm;
		case MTLPixelFormatRGBA8Uint:
			return Format::R8G8B8A8Uint;
		case MTLPixelFormatRGBA8Sint:
			return Format::R8G8B8A8Sint;
		case MTLPixelFormatRGBA8Unorm_sRGB:
			return Format::R8G8B8A8Srgb;
		case MTLPixelFormatBGRA8Unorm:
			return Format::B8G8R8A8Unorm;
		case MTLPixelFormatBGRA8Unorm_sRGB:
			return Format::B8G8R8A8Srgb;
		case MTLPixelFormatRGB10A2Unorm:
			return Format::A2R10G10B10UnormPack32;
		case MTLPixelFormatRGB10A2Uint:
			return Format::A2R10G10B10UintPack32;
		case MTLPixelFormatBGR10A2Unorm:
			return Format::A2B10G10R10UnormPack32;
		case MTLPixelFormatR16Unorm:
			return Format::R16Unorm;
		case MTLPixelFormatR16Snorm:
			return Format::R16Snorm;
		case MTLPixelFormatR16Uint:
			return Format::R16Uint;
		case MTLPixelFormatR16Sint:
			return Format::R16Sint;
		case MTLPixelFormatR16Float:
			return Format::R16Sfloat;
		case MTLPixelFormatRG16Unorm:
			return Format::R16G16Unorm;
		case MTLPixelFormatRG16Snorm:
			return Format::R16G16Snorm;
		case MTLPixelFormatRG16Uint:
			return Format::R16G16Uint;
		case MTLPixelFormatRG16Sint:
			return Format::R16G16Sint;
		case MTLPixelFormatRG16Float:
			return Format::R16G16Sfloat;
		case MTLPixelFormatRGBA16Unorm:
			return Format::R16G16B16A16Unorm;
		case MTLPixelFormatRGBA16Snorm:
			return Format::R16G16B16A16Snorm;
		case MTLPixelFormatRGBA16Uint:
			return Format::R16G16B16A16Uint;
		case MTLPixelFormatRGBA16Sint:
			return Format::R16G16B16A16Sint;
		case MTLPixelFormatRGBA16Float:
			return Format::R16G16B16A16Sfloat;
		case MTLPixelFormatR32Uint:
			return Format::R32Uint;
		case MTLPixelFormatR32Sint:
			return Format::R32Sint;
		case MTLPixelFormatR32Float:
			return Format::R32Sfloat;
		case MTLPixelFormatRG32Uint:
			return Format::R32G32Uint;
		case MTLPixelFormatRG32Sint:
			return Format::R32G32Sint;
		case MTLPixelFormatRG32Float:
			return Format::R32G32Sfloat;
		case MTLPixelFormatRGBA32Uint:
			return Format::R32G32B32A32Uint;
		case MTLPixelFormatRGBA32Sint:
			return Format::R32G32B32A32Sint;
		case MTLPixelFormatRGBA32Float:
			return Format::R32G32B32A32Sfloat;
		case MTLPixelFormatDepth16Unorm:
			return Format::D16Unorm;
		case MTLPixelFormatDepth24Unorm_Stencil8:
			return Format::D24UnormS8Uint;
		case MTLPixelFormatDepth32Float:
			return Format::D32Sfloat;
		case MTLPixelFormatDepth32Float_Stencil8:
			return Format::D32SfloatS8Uint;
		default:
			return Format::Undefined;
	}
}

MTLTextureType textureTypeMtl(const TextureType& type) {
	switch(type){
		case TextureType::Texture1D:
			return MTLTextureType1D;
		case TextureType::Texture1DArray:
			return MTLTextureType1DArray;
		case TextureType::Texture2D:
			return MTLTextureType2D;
		case TextureType::Texture2DArray:
			return MTLTextureType2DArray;
		case TextureType::Texture3D:
			return MTLTextureType3D;
		case TextureType::TextureCube:
			return MTLTextureTypeCube;
		case TextureType::TextureCubeArray:
			return MTLTextureTypeCubeArray;
	}
}
