//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_IMAGE_HPP
#define MANGO_IMAGE_HPP

#include <memory>
#include "Types.hpp"
#include "Device.hpp"

namespace mango {

class Texture;
typedef std::shared_ptr<Texture> spTexture;

class TextureView {
	public:
		virtual ~TextureView() = default;
		TextureView() = default;
		TextureView(const spTexture& texture) : _texture(texture) {}

		spTexture getTexture() const {return _texture.lock();}
	private:
		std::weak_ptr<Texture> _texture;
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

		virtual void create(int width,int height,
							int mipLevels, const Format& format, const TextureUsage& usage) = 0;
		virtual void create3D(int width,int height, int depth, int miplevels, const Format& format, const TextureUsage& usage) = 0;
		virtual void createCubeMap(int width, int height, int mipLevels, const Format& format, const TextureUsage& usage) = 0;

		virtual spTextureView createTextureView(const ComponentMapping& componentMapping = ComponentMapping(),int minLayer = 0, int maxLayer = -1,int minLevel = 0,int maxLevel = -1) = 0;
		virtual spTextureView createTextureViewCubeMap(const ComponentMapping& componentMapping = ComponentMapping(),int minLayer = 0, int maxLayer = -1,int minLevel = 0,int maxLevel = -1) = 0;

		virtual void set(const spBuffer& buffer) = 0;

		inline int width() { return _width; }
		inline int height() { return _height; }
		inline int depth() { return _depth; }
		inline int mipLevels() { return _mipLevels; }
		inline TextureUsage type() { return _usage; }
		inline TextureLayout layout() { return _layout; }
		inline Format format() { return _format; }

		virtual void transition(const TextureLayout& layout,const spCommandBuffer& cmd = nullptr) = 0;

		virtual void generationMipMaps() = 0;
	protected:
		int _width;
		int _height;
		int _depth = 1;
		int _layers = 1;
		int _mipLevels;
		Format _format;
		TextureUsage _usage;
		TextureLayout _layout;
};

inline bool hasDepthComponent(Format format) {
    return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint
        || format == Format::D16Unorm || format == Format::D16UnormS8Uint || format == Format::D32Sfloat;
}

};

#endif //MANGO_IMAGE_HPP
