//
//  TextureTypes.h
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//

#pragma once

#include <string>

namespace mango {

enum class Format {
    Undefined,
    R8Unorm,
    R8Snorm,
    R8Uint,
    R8Sint,
    R8G8B8A8Unorm,
    R8G8B8A8Snorm,
    R8G8B8A8Uint,
    R8G8B8A8Sint,
    R8G8B8A8Srgb,
    B8G8R8A8Unorm,
    B8G8R8A8Srgb,
    A2R10G10B10UnormPack32,
    A2R10G10B10UintPack32,
    A2B10G10R10UnormPack32,
    R16Unorm,
    R16Snorm,
    R16Uint,
    R16Sint,
    R16Sfloat,
    R16G16Unorm,
    R16G16Snorm,
    R16G16Uint,
    R16G16Sint,
    R16G16Sfloat,
    R16G16B16A16Unorm,
    R16G16B16A16Snorm,
    R16G16B16A16Uint,
    R16G16B16A16Sint,
    R16G16B16A16Sfloat,
    R32Uint,
    R32Sint,
    R32Sfloat,
    R32G32Uint,
    R32G32Sint,
    R32G32Sfloat,
    R32G32B32A32Uint,
    R32G32B32A32Sint,
    R32G32B32A32Sfloat,
    D16Unorm,
    D24UnormS8Uint,
    D32Sfloat,
    D32SfloatS8Uint
};

inline uint32_t pixelSize(Format format) {
    switch(format){
        case Format::Undefined:
            return 0;
        case Format::R8Unorm:
        case Format::R8Snorm:
        case Format::R8Uint:
        case Format::R8Sint:
            return 1;
        case Format::R8G8B8A8Unorm:
        case Format::R8G8B8A8Snorm:
        case Format::R8G8B8A8Uint:
        case Format::R8G8B8A8Sint:
        case Format::R8G8B8A8Srgb:
        case Format::B8G8R8A8Unorm:
        case Format::B8G8R8A8Srgb:
        case Format::A2R10G10B10UnormPack32:
        case Format::A2R10G10B10UintPack32:
        case Format::A2B10G10R10UnormPack32:
            return 4;
        case Format::R16Unorm:
        case Format::R16Snorm:
        case Format::R16Uint:
        case Format::R16Sint:
        case Format::R16Sfloat:
        case Format::D16Unorm:
            return 2;
        case Format::R16G16Unorm:
        case Format::R16G16Snorm:
        case Format::R16G16Uint:
        case Format::R16G16Sint:
        case Format::R16G16Sfloat:
        case Format::D24UnormS8Uint:
        case Format::D32Sfloat:
            return 4;
    	case Format::D32SfloatS8Uint:
    		return 5;
        case Format::R16G16B16A16Unorm:
        case Format::R16G16B16A16Snorm:
        case Format::R16G16B16A16Uint:
        case Format::R16G16B16A16Sint:
        case Format::R16G16B16A16Sfloat:
            return 8;
        case Format::R32Uint:
        case Format::R32Sint:
        case Format::R32Sfloat:
            return 4;
        case Format::R32G32Uint:
        case Format::R32G32Sint:
        case Format::R32G32Sfloat:
            return 8;
        case Format::R32G32B32A32Uint:
        case Format::R32G32B32A32Sint:
        case Format::R32G32B32A32Sfloat:
            return 16;
    }
}

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

inline bool hasDepthComponent(Format format) {
	return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint
		   || format == Format::D16Unorm || format == Format::D32Sfloat;
}

inline bool hasStencilComponent(Format format) {
	return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint;
}

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

}
