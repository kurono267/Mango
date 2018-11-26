//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_IMAGE_VK_HPP
#define MANGO_IMAGE_VK_HPP

#include "default.hpp"
#include "api/texture.hpp"
#include "convert_vk.hpp"

namespace mango::vulkan {

class TextureVK : public Texture {
	public:
		TextureVK() = default;
		~TextureVK() final;

		void create(const spDevice& device,int width, int height,int miplevels,const Format& format, const TextureType &type, const void *data) override;
		void create(const vk::Device& device,int width,int height,int miplevels,const Format& format,const TextureType &type,const vk::Image& image);
		spTextureView createTextureView(const ComponentSwizzle& swizzle = ComponentSwizzle(),int minLevel = 0,int maxLevel = -1) override;
	protected:
		vk::Device _vk_device;
		vk::Image _image;
		vk::DeviceMemory _memory;
};

class TextureViewVK : public TextureView {
	friend class TextureVK;
	public:
		TextureViewVK() = default;
		TextureViewVK(const spTexture& texture) : mango::TextureView(texture) {}
		~TextureViewVK() final;

		vk::ImageView getView();
	private:
		vk::ImageView _view;
		bool _isInit = false;
};

typedef std::shared_ptr<TextureVK> spTextureVK;

};

#endif //MANGO_IMAGE_VK_HPP
