//
// Created by kurono267 on 06.07.18.
//

#include "TextureVK.hpp"
#include "BufferVK.hpp"
#include "../api/Instance.hpp"
#include "../vk/DeviceVK.hpp"
#include <iostream>
#include "../api/Utils.hpp"

using namespace mango::vulkan;

TextureViewVK::~TextureViewVK(){
	Instance::device<DeviceVK>()->getDevice().destroyImageView(_view);
}

vk::ImageView TextureViewVK::getView(){
	return _view;
}

TextureVK::~TextureVK() {
	std::cout << "~TextureVK" << std::endl;
	if(_isOwned){
		Instance::device<DeviceVK>()->getDevice().destroyImage(_image);
		if(_memory){
			Instance::device<DeviceVK>()->getDevice().free(_memory);
		}
	}
}

void TextureVK::create(const int width,const int height,const int miplevels,const Format& format,const mango::TextureType &type, const vk::Image& image){
    auto internal = Instance::device<DeviceVK>();

    _pool = internal->getCommandPool();
    _queue = internal->getGraphicsQueue();
    _image = image;
    _width = width;
    _height = height;
    _format = format;
    _mipLevels = miplevels;
    _type = type;
	_isOwned = false;
	_imageType = vk::ImageType::e2D;
}

void TextureVK::create(const int width, const int height,const int miplevels , const Format& format, const mango::TextureType &type) {
	auto device = Instance::device<DeviceVK>();
	auto vkDevice = device->getDevice();
	_width = width;
	_height = height;
	_depth = 1;
	_mipLevels = miplevels;
	_layers = 1;
	_type = type;
	_format = format;
	_isOwned = true;
	_imageType = vk::ImageType::e2D;

	vk::ImageUsageFlags usage = (vk::ImageUsageFlags)0;
	vk::ImageLayout layout = vk::ImageLayout::ePreinitialized;
	if(type == mango::TextureType::DepthStencil){
		usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		layout = vk::ImageLayout::eUndefined;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Input){
		usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Output) {
		usage |= vk::ImageUsageFlagBits::eColorAttachment;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Storage){
		usage |= vk::ImageUsageFlagBits::eStorage;
	}

	createVK(vk::ImageCreateFlags(),_imageType,vk::Extent3D(_width,_height,_depth),_layers,_mipLevels,mango::vulkan::formatVK(format),usage,layout);

	if(((uint32_t)type & (uint32_t)mango::TextureType::Output)
	   && ((uint32_t)type & (uint32_t)mango::TextureType::Input)){
		transition(vk::ImageLayout::eShaderReadOnlyOptimal);
	}
	if(type == mango::TextureType::DepthStencil){
		transition(vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}
}

void TextureVK::create3D(int width,int height, int depth, int miplevels, const Format& format, const TextureType& type) {
	auto device = Instance::device<DeviceVK>();
	auto vkDevice = device->getDevice();
	_width = width;
	_height = height;
	_depth = depth;
	_mipLevels = miplevels;
	_layers = 1;
	_type = type;
	_format = format;
	_isOwned = true;
	_imageType = vk::ImageType::e3D;

	vk::ImageUsageFlags usage = (vk::ImageUsageFlags)0;
	vk::ImageLayout layout = vk::ImageLayout::ePreinitialized;
	if(type == mango::TextureType::DepthStencil){
		usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		layout = vk::ImageLayout::eUndefined;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Input){
		usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Output) {
		usage |= vk::ImageUsageFlagBits::eColorAttachment;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Storage){
		usage |= vk::ImageUsageFlagBits::eStorage;
	}

	createVK(vk::ImageCreateFlags(),vk::ImageType::e3D,vk::Extent3D(_width,_height,_depth),_layers,_mipLevels,mango::vulkan::formatVK(format),usage,layout);

	if(((uint32_t)type & (uint32_t)mango::TextureType::Output)
	   && ((uint32_t)type & (uint32_t)mango::TextureType::Input)){
		transition(vk::ImageLayout::eShaderReadOnlyOptimal);
	}
	if(type == mango::TextureType::DepthStencil){
		transition(vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}
}

void TextureVK::createCubeMap(int width, int height, int mipLevels, const Format& format, const TextureType& type) {
	auto device = Instance::device<DeviceVK>();
	auto vkDevice = device->getDevice();
	_width = width;
	_height = height;
	_depth = 1;
	_mipLevels = mipLevels;
	_layers = 6;
	_type = type;
	_format = format;
	_isOwned = true;
	_imageType = vk::ImageType::e2D;

	vk::ImageUsageFlags usage = (vk::ImageUsageFlags)0;
	vk::ImageLayout layout = vk::ImageLayout::ePreinitialized;
	if(type == mango::TextureType::DepthStencil){
		usage |= vk::ImageUsageFlagBits::eDepthStencilAttachment;
		layout = vk::ImageLayout::eUndefined;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Input){
		usage |= vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Output) {
		usage |= vk::ImageUsageFlagBits::eColorAttachment;
	}
	if((uint32_t)type & (uint32_t)mango::TextureType::Storage){
		usage |= vk::ImageUsageFlagBits::eStorage;
	}

	createVK(vk::ImageCreateFlagBits::eCubeCompatible,_imageType,vk::Extent3D(_width,_height,_depth),_layers,_mipLevels,mango::vulkan::formatVK(format),usage,layout);

	if(((uint32_t)type & (uint32_t)mango::TextureType::Output)
	   && ((uint32_t)type & (uint32_t)mango::TextureType::Input)){
		transition(vk::ImageLayout::eShaderReadOnlyOptimal);
	}
	if(type == mango::TextureType::DepthStencil){
		transition(vk::ImageLayout::eDepthStencilAttachmentOptimal);
	}
}

void TextureVK::createVK(const vk::ImageCreateFlags& flags,const vk::ImageType& imageType,const vk::Extent3D& extent3D,const int layers, const int mipLevels, const vk::Format& format, const vk::ImageUsageFlags& usage,const vk::ImageLayout& layout) {
	auto device = Instance::device<DeviceVK>();
	auto vkDevice = device->getDevice();

	vk::ImageCreateInfo imageInfo(
			flags, // Basic
			imageType, // Type 1D,2D,3D
			format, // Format
			extent3D, // Width, Height and Depth
			mipLevels, // Mip Levels
			layers, // Array Layers
			vk::SampleCountFlagBits::e1, // Samples
			vk::ImageTiling::eOptimal,
			usage,
			vk::SharingMode::eExclusive, 0, nullptr, layout
	);

	_pool = device->getCommandPool();
	_queue = device->getGraphicsQueue();

	// Create Image
	_image = vkDevice.createImage(imageInfo);
	auto memoryReq =vkDevice.getImageMemoryRequirements(_image);

	vk::MemoryPropertyFlags memory_type = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vk::MemoryAllocateInfo allocInfo(memoryReq.size,findMemoryType(device->getPhysicalDevice(),memoryReq.memoryTypeBits, memory_type));
	_memory = vkDevice.allocateMemory(allocInfo);

	vkDevice.bindImageMemory(_image,_memory,0);
}

void flagsFromLayout(const vk::ImageLayout& layout,vk::AccessFlags& accessFlag,vk::PipelineStageFlags& stage){
	switch(layout){
		case vk::ImageLayout::ePreinitialized:
			accessFlag = vk::AccessFlagBits::eHostWrite;
			stage      = vk::PipelineStageFlagBits::eHost;
			break;
		case vk::ImageLayout::eTransferDstOptimal:
			accessFlag = vk::AccessFlagBits::eTransferWrite;
			stage      = vk::PipelineStageFlagBits::eTransfer;
			break;
		case vk::ImageLayout::eShaderReadOnlyOptimal:
			accessFlag = vk::AccessFlagBits::eShaderRead;
			stage      = vk::PipelineStageFlagBits::eFragmentShader;
			break;
		case vk::ImageLayout::eUndefined:
			accessFlag = (vk::AccessFlagBits)0;
			stage      = vk::PipelineStageFlagBits::eTopOfPipe;
			break;
		case vk::ImageLayout::eGeneral:
			accessFlag = vk::AccessFlagBits::eShaderRead|vk::AccessFlagBits::eShaderWrite;
			stage      = vk::PipelineStageFlagBits::eFragmentShader;
			break;
		case vk::ImageLayout::eTransferSrcOptimal:
			accessFlag = vk::AccessFlagBits::eTransferRead;
			stage      = vk::PipelineStageFlagBits::eTransfer;
			break;
		case vk::ImageLayout::eColorAttachmentOptimal:
			accessFlag = vk::AccessFlagBits::eColorAttachmentWrite;
			stage      = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			break;
		case vk::ImageLayout::eDepthStencilAttachmentOptimal:
			accessFlag = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
			stage      = vk::PipelineStageFlagBits::eEarlyFragmentTests;
			break;
		case vk::ImageLayout::ePresentSrcKHR:
			accessFlag = vk::AccessFlagBits::eColorAttachmentWrite;
			stage      = vk::PipelineStageFlagBits::eColorAttachmentOutput;
			break;
		default:
			throw std::runtime_error("flagsFromLayout not supported this ImageLayout");
	}
}

void TextureVK::transition(const vk::ImageLayout& newLayout){
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vk::CommandBuffer commandBuffer = beginSingle(vkDevice,_pool);

	vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;
	if (newLayout == vk::ImageLayout::eDepthStencilAttachmentOptimal) {
		aspectMask = vk::ImageAspectFlagBits::eDepth;

		if (hasStencilComponent(_format)) {
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}

	vk::AccessFlags srcAccess;
	vk::AccessFlags dstAccess;
	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
	flagsFromLayout(_layout,srcAccess,srcStage);
	flagsFromLayout(newLayout,dstAccess,dstStage);

	vk::ImageSubresourceRange subRes(
			aspectMask,
			0, _mipLevels, /* Mip levels current and count*/ 0, 1 /* Layers current and count */ );

	vk::ImageMemoryBarrier barrier(
			srcAccess,
			dstAccess,
			_layout,
			newLayout,
			VK_QUEUE_FAMILY_IGNORED,
			VK_QUEUE_FAMILY_IGNORED,
			_image,
			subRes
	);

	commandBuffer.pipelineBarrier(
			srcStage, dstStage,
			vk::DependencyFlagBits::eByRegion,
			0, nullptr,
			0, nullptr,
			1, &barrier
	);

	endSingle(vkDevice,_queue,_pool,commandBuffer);
	_layout = newLayout;
}

mango::spTextureView TextureVK::createTextureView(const ComponentMapping& componentMapping,int minLayer, int maxLayer,int minLevel,int maxLevel){
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(_format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;

	vk::ImageViewType viewType = vk::ImageViewType::e2D;
	if(_imageType == vk::ImageType::e3D){
		viewType = vk::ImageViewType::e3D;
	}
	auto viewCreateInfo = vk::ImageViewCreateInfo(
		vk::ImageViewCreateFlags(),
		_image,
		viewType,
		formatVK(_format),
		componentMappingVK(componentMapping),
		vk::ImageSubresourceRange(
			imageAspectFlags,
			minLevel, maxLevel!=-1?maxLevel:_mipLevels, minLayer, maxLayer!=-1?maxLayer:_layers)
	);
	auto texView = std::make_shared<TextureViewVK>(shared_from_this());
	texView->_view = vkDevice.createImageView(viewCreateInfo);
	texView->_isInit = true;
	return texView;
}

mango::spTextureView TextureVK::createTextureViewCubeMap(const ComponentMapping& componentMapping,int minLayer, int maxLayer,int minLevel,int maxLevel){
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(_format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;
	auto viewCreateInfo = vk::ImageViewCreateInfo(
			vk::ImageViewCreateFlags(),
			_image,
			vk::ImageViewType::eCube,
			formatVK(_format),
			componentMappingVK(componentMapping),
			vk::ImageSubresourceRange(
					imageAspectFlags,
					minLevel, maxLevel!=-1?maxLevel:_mipLevels, minLayer, maxLayer!=-1?maxLayer:_layers)
	);
	auto texView = std::make_shared<TextureViewVK>(shared_from_this());
	texView->_view = vkDevice.createImageView(viewCreateInfo);
	texView->_isInit = true;
	return texView;
}

void TextureVK::set(const mango::spBuffer &buffer) {
	transition(vk::ImageLayout::eTransferDstOptimal);
    setBuffer(buffer,glm::ivec3(_width,_height,_depth),0,0,0);
    transition(vk::ImageLayout::eShaderReadOnlyOptimal);
}

void TextureVK::setBuffer(const spBuffer& buffer, const glm::ivec3& size, const uint& mipLevel, const uint& layer, const uint& offsetBuffer){
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
    vk::CommandBuffer commandBuffer = beginSingle(vkDevice,_pool);

    vk::ImageSubresourceLayers subRes(
            vk::ImageAspectFlagBits::eColor,
            mipLevel, /* Mip levels current*/ layer, 1 /* Layers current and count */ );

    vk::BufferImageCopy region(
            /* bufferOffset bufferRowLength bufferImageHeight */
            offsetBuffer, 0, 0,
            /* subRes imageOffest */
            subRes, {0, 0, 0},
            /* imageExtent */
            {(uint)size.x,(uint)size.y,(uint)size.z}
    );

    commandBuffer.copyBufferToImage(std::dynamic_pointer_cast<BufferVK>(buffer)->getVKBuffer(),_image,vk::ImageLayout::eTransferDstOptimal, {region});

    endSingle(vkDevice,_queue,_pool,commandBuffer);
}

vk::Image TextureVK::getImage() {
	return _image;
}

vk::ImageLayout TextureVK::getImageLayout() {
	return _layout;
}

vk::Sampler mango::vulkan::createSampler(const mango::Sampler &sampler) {
	vk::SamplerCreateInfo createInfo;
	createInfo.addressModeU = samplerAddressModeVK(sampler.addressModeU);
	createInfo.addressModeV = samplerAddressModeVK(sampler.addressModeV);
	createInfo.addressModeW = samplerAddressModeVK(sampler.addressModeW);
	createInfo.anisotropyEnable = (VkBool32)sampler.anisotropyEnable;
	createInfo.borderColor = borderColorVK(sampler.borderColor);
	createInfo.compareEnable = (VkBool32)sampler.compareEnable;
	createInfo.compareOp = compareOpVK(sampler.compareOp);
	createInfo.magFilter = filterVK(sampler.magFilter);
	createInfo.minFilter = filterVK(sampler.minFilter);
	createInfo.maxAnisotropy = sampler.maxAnisotropy;
	createInfo.maxLod = sampler.maxLod;
	createInfo.minLod = sampler.minLod;
	createInfo.mipLodBias = sampler.mipLodBias;
	createInfo.mipmapMode = samplerMipmapModeVK(sampler.mipmapMode);
	return Instance::device<DeviceVK>()->getDevice().createSampler(createInfo);
}
