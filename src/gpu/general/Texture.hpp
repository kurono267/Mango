//
//  Texture.hpp
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//
#pragma once

#include "TextureTypes.hpp"
#include "RenderPipelineTypes.hpp"
#include "third_party.hpp"

namespace mango {

enum class TextureType {
	Texture2D,
	Texture3D,
	TextureCube
};

struct Rect2D {
    Rect2D() = default;
    Rect2D(const glm::ivec2& offset, const glm::ivec2& size);
    Rect2D(int x, int y, int width, int height);

    glm::ivec2 offset;
    glm::ivec2 size;
};

struct TextureLayout {
    TextureLayout(TextureType type, uint32_t width, uint32_t height, uint32_t depth, Format format, uint32_t mipLevels = 1, uint32_t layers = 1);
    
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t depth = 1;

    uint32_t mipLevels = 1;
    uint32_t layers = 1;

    Format format;
    TextureType type;
};

struct Sampler {
	explicit Sampler(Filter magFilter_ = Filter::Linear,
			Filter minFilter_ = Filter::Linear,
			SamplerMipmapMode mipmapMode_ = SamplerMipmapMode::Nearest,
			SamplerAddressMode addressModeU_ = SamplerAddressMode::Repeat,
			SamplerAddressMode addressModeV_ = SamplerAddressMode::Repeat,
			SamplerAddressMode addressModeW_ = SamplerAddressMode::Repeat,
			float mipLodBias_ = 0,
			bool anisotropyEnable_ = false,
			float maxAnisotropy_ = 0,
			bool compareEnable_ = false,
			CompareOp compareOp_ = CompareOp::Never,
			float minLod_ = 0,
			float maxLod_ = 0,
			BorderColor borderColor_ = BorderColor::FloatTransparentBlack,
			bool unnormalizedCoordinates_ = false)
			: magFilter( magFilter_ )
			, minFilter( minFilter_ )
			, mipmapMode( mipmapMode_ )
			, addressModeU( addressModeU_ )
			, addressModeV( addressModeV_ )
			, addressModeW( addressModeW_ )
			, mipLodBias( mipLodBias_ )
			, anisotropyEnable( anisotropyEnable_ )
			, maxAnisotropy( maxAnisotropy_ )
			, compareEnable( compareEnable_ )
			, compareOp( compareOp_ )
			, minLod( minLod_ )
			, maxLod( maxLod_ )
			, borderColor( borderColor_ )
			, unnormalizedCoordinates( unnormalizedCoordinates_ ) {}

	Filter magFilter;
	Filter minFilter;
	SamplerMipmapMode mipmapMode;
	SamplerAddressMode addressModeU;
	SamplerAddressMode addressModeV;
	SamplerAddressMode addressModeW;
	float mipLodBias;
	bool anisotropyEnable;
	float maxAnisotropy;
	bool compareEnable;
	CompareOp compareOp;
	float minLod;
	float maxLod;
	BorderColor borderColor;
	bool unnormalizedCoordinates;
};

size_t bufferSize(const TextureLayout& layout);

}
