//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_IMAGE_VK_HPP
#define MANGO_IMAGE_VK_HPP

#include "default.hpp"
#include "api/Texture.hpp"
#include "ConvertVK.hpp"

namespace mango::vulkan {

class TextureVK : public Texture {
	public:
		TextureVK() = default;
		~TextureVK() final;

		void create(int width, int height,int miplevels,const Format& format, const TextureType &type) override;
	    void create(int width,int height,int miplevels,const Format& format,const TextureType &type,const vk::Image& image);
		spTextureView createTextureView(const ComponentMapping& componentMapping = ComponentMapping(),int minLevel = 0,int maxLevel = -1) override;

	    void set(const spBuffer &buffer) override;

	    void transition(const vk::ImageLayout& newLayout);

        void setBuffer(const spBuffer& buffer, const glm::ivec2& size, const uint& mipLevel, const uint& layer, const uint& offsetBuffer);

        vk::Image getImage();
        vk::ImageLayout getImageLayout();
protected:
		vk::Image _image;
		vk::DeviceMemory _memory;
		vk::CommandPool _pool;
		vk::Queue _queue;
		vk::ImageLayout _layout;
		bool _isOwned;
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
typedef std::shared_ptr<TextureViewVK> spTextureViewVK;

vk::Sampler createSampler(const Sampler& sampler);

};

#endif //MANGO_IMAGE_VK_HPP
