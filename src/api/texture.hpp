//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_IMAGE_HPP
#define MANGO_IMAGE_HPP

#include <memory>
#include "types.hpp"
#include "device.hpp"

namespace mango {

class TextureView {
	public:
		TextureView() = default;
		virtual ~TextureView() = default;
};

typedef std::shared_ptr<TextureView> spTextureView;

class Texture {
	public:
		Texture() = default;
		virtual ~Texture() = default;

		virtual void create(const spDevice& device,const int width,const int height,
							const int miplevels, const Format& format,const TextureType& type, const void* data = nullptr) = 0;
		virtual spTextureView createTextureView(const ComponentSwizzle& swizzle = ComponentSwizzle(),const int minLevel = 0,const int maxLevel = -1) = 0;

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

typedef std::shared_ptr<Texture> spTexture;

inline bool hasDepthComponent(Format format) {
    return format == Format::D32SfloatS8Uint || format == Format::D24UnormS8Uint
        || format == Format::D16Unorm || format == Format::D16UnormS8Uint || format == Format::D32Sfloat;
}

};

#endif //MANGO_IMAGE_HPP
