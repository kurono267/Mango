//
// Created by kurono267 on 06.07.18.
//

#include "texture_vk.hpp"
#include "buffer_vk.hpp"

using namespace mango::vulkan;

TextureViewVK::~TextureViewVK(){

}

vk::ImageView TextureViewVK::getView(){
	return _view;
}

TextureVK::~TextureVK() {

}

void TextureVK::create(const spDevice& device,const int width, const int height,const int miplevels , const Format& format, const mango::TextureType &type, const void *data) {
	_width = width;
	_height = height;
	_mipLevels = miplevels;
	_type = type;
	_format = format;

	vk::ImageUsageFlags usage;
	switch(type){
		case mango::TextureType::Input:
		usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eColorAttachment;
		break;
		case mango::TextureType::Output:
		usage = vk::ImageUsageFlagBits::eColorAttachment;
		break;
	}

	vk::ImageLayout layout = vk::ImageLayout::ePreinitialized;

	vk::ImageCreateInfo imageInfo(
		vk::ImageCreateFlags(), // Basic
		vk::ImageType::e2D, // Type 1D,2D,3D
		mango::vulkan::formatVK(_format), // Format
		vk::Extent3D(_width,_height,1), // Width, Height and Depth
		_mipLevels, // Mip Levels
		1, // Array Layers
		vk::SampleCountFlagBits::e1, // Samples
		vk::ImageTiling::eOptimal,
		usage,
		vk::SharingMode::eExclusive, 0, nullptr, layout
	);

	auto mango = std::dynamic_pointer_cast<DeviceVK>(device);
	auto vk_device = mango->getDevice();
	_vk_device = vk_device;

	// Create Image
	_image = vk_device.createImage(imageInfo);
	auto memoryReq =vk_device.getImageMemoryRequirements(_image);

	vk::MemoryPropertyFlags memory_type = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vk::MemoryAllocateInfo allocInfo(memoryReq.size,findMemoryType(mango->getPhysicalDevice(),memoryReq.memoryTypeBits, memory_type));
	_memory = vk_device.allocateMemory(allocInfo);

	vk_device.bindImageMemory(_image,_memory,0);
}

mango::spTextureView TextureVK::createTextureView(const ComponentSwizzle& swizzle,const int minLevel,const int maxLevel){
	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(_format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;
	auto viewCreateInfo = vk::ImageViewCreateInfo(
		vk::ImageViewCreateFlags(),
		_image,
		vk::ImageViewType::e2D,
		formatVK(_format),
		vk::ComponentMapping(),
		vk::ImageSubresourceRange(
			imageAspectFlags,
			minLevel, maxLevel!=-1?maxLevel:_mipLevels, 0, 1)
	);
	auto texView = std::make_shared<TextureViewVK>();
	texView->_view = _vk_device.createImageView(viewCreateInfo);
	texView->_isInit = true;
	return texView;
}
