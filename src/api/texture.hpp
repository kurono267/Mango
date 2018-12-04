//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_IMAGE_HPP
#define MANGO_IMAGE_HPP

#include <memory>
#include "types.hpp"
#include "device.hpp"

namespace mango {

class Texture;
typedef std::shared_ptr<Texture> spTexture;

class TextureView {
	public:
		virtual ~TextureView() = default;
		TextureView() = default;
		TextureView(const spTexture& texture) : _texture(texture) {}

		spTexture getTexture() const {return _texture;}
	private:
		spTexture _texture;
};

struct Sampler {
	Sampler(Filter magFilter_ = Filter::Linear,
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
			bool unnormalizedCoordinates_ = 0)
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

typedef std::shared_ptr<TextureView> spTextureView;

class Texture : public std::enable_shared_from_this<Texture> {
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual void create(const spDevice& device,int width,int height,
							int miplevels, const Format& format,const TextureType& type, const void* data = nullptr) = 0;
		virtual spTextureView createTextureView(const ComponentSwizzle& swizzle = ComponentSwizzle(),int minLevel = 0,int maxLevel = -1) = 0;

		inline int width() { return _width; }
		inline int height() { return _height; }
		inline int mipLevels() { return _mipLevels; }
		inline TextureType type() { return _type; }
		inline Format format() { return _format; }
	protected:
		int _width;
		int _height;
		int _mipLevels;
		Format _format;
		TextureType _type;
};

inline bool hasDepthComponent(Format format) {
    return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint
        || format == Format::D16Unorm || format == Format::D16UnormS8Uint || format == Format::D32Sfloat;
}

};

#endif //MANGO_IMAGE_HPP
