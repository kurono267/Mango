//
// Created by kurono267 on 06.07.18.
//

#include "TextureVK.hpp"
#include "BufferVK.hpp"
#include "../api/Instance.hpp"
#include "DeviceVK.hpp"
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

void TextureVK::create(const int width,const int height,const int miplevels,const Format& format,const TextureUsage& usage, const vk::Image& image){
    auto internal = Instance::device<DeviceVK>();

    _pool = internal->getCommandPool();
    _queue = internal->getGraphicsQueue();
    _image = image;
    _width = width;
    _height = height;
    _format = format;
    _mipLevels = miplevels;
    _usage = usage;
    _layout = TextureLayout::Undefined;
	_isOwned = false;
	_imageType = vk::ImageType::e2D;
}

void TextureVK::createInternal(vk::ImageType type,int width,int height, int depth, int miplevels, int layers, const Format& format, const TextureUsage& usage) {
	_width = width;
	_height = height;
	_depth = depth;
	_mipLevels = miplevels;
	_layers = layers;
	_usage = usage;
	_format = format;
	_isOwned = true;
	_imageType = type;

	if(((uint32_t)_usage & (uint32_t)mango::TextureUsage::DepthStencilAttachment)){
		_layout = TextureLayout::Undefined;
	} else if(((uint32_t)_usage & (uint32_t)mango::TextureUsage::Sampled)){
		_layout = TextureLayout::Preinitialized;
	}

	vk::ImageUsageFlags vkUsage = imageUsageVK(_usage);
	vk::ImageLayout vkLayout = imageLayoutVK(_layout);

	createVK(vk::ImageCreateFlags(),_imageType,vk::Extent3D(_width,_height,_depth),_layers,_mipLevels,mango::vulkan::formatVK(format),vkUsage,vkLayout);

	if(((uint32_t)_usage & (uint32_t)mango::TextureUsage::DepthStencilAttachment)){
		transition(TextureLayout::DepthStencilAttachmentOptimal);
	} else if(((uint32_t)_usage & (uint32_t)mango::TextureUsage::Sampled)){
		transition(TextureLayout::ShaderReadOnlyOptimal);
	}
}

void TextureVK::create(const int width, const int height,const int miplevels , const Format& format, const TextureUsage& usage) {
	createInternal(vk::ImageType::e2D,width,height,1,miplevels,1,format,usage);
}

void TextureVK::create3D(int width,int height, int depth, int miplevels, const Format& format, const TextureUsage& usage) {
	createInternal(vk::ImageType::e3D,width,height,depth,miplevels,1,format,usage);
}

void TextureVK::createCubeMap(int width, int height, int mipLevels, const Format& format, const TextureUsage& usage) {
	createInternal(vk::ImageType::e2D,width,height,1,mipLevels,6,format,usage);
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

void TextureVK::transition(const TextureLayout& newLayout,const spCommandBuffer& cmd)  {
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vk::CommandBuffer commandBuffer;
	if(!cmd) {
		commandBuffer = beginSingle(vkDevice, _pool);
	} else {
		commandBuffer = std::static_pointer_cast<CommandBufferVK>(cmd)->getVK();
	}

	vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(_format)){
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (hasStencilComponent(_format)) {
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}

	vk::AccessFlags srcAccess;
	vk::AccessFlags dstAccess;
	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
	flagsFromLayout(imageLayoutVK(_layout),srcAccess,srcStage);
	flagsFromLayout(imageLayoutVK(newLayout),dstAccess,dstStage);

	vk::ImageSubresourceRange subRes(
			aspectMask,
			0, _mipLevels, /* Mip levels current and count*/ 0, 1 /* Layers current and count */ );

	vk::ImageMemoryBarrier barrier(
			srcAccess,
			dstAccess,
			imageLayoutVK(_layout),
			imageLayoutVK(newLayout),
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

	if(!cmd) {
		endSingle(vkDevice,_queue,_pool,commandBuffer);
	}
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
	transition(TextureLayout::TransferDstOptimal);
    setBuffer(buffer,glm::ivec3(_width,_height,_depth),0,0,0);
    transition(TextureLayout::ShaderReadOnlyOptimal);
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

void TextureVK::generationMipMaps() {
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();

	transition(TextureLayout::TransferDstOptimal);

	vk::CommandBuffer commandBuffer = beginSingle(vkDevice,_pool);

	VkImageMemoryBarrier barrier{};
	barrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
	barrier.image = _image;
	barrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
	barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
	barrier.subresourceRange.baseArrayLayer = 0;
	barrier.subresourceRange.layerCount = 1;
	barrier.subresourceRange.levelCount = 1;

	int32_t mipWidth = _width;
	int32_t mipHeight = _height;

	for (uint32_t i = 1; i < _mipLevels; i++) {
		barrier.subresourceRange.baseMipLevel = i - 1;
		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
		barrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0,
							 0, nullptr,
							 0, nullptr,
							 1, &barrier);

		VkImageBlit blit{};
		blit.srcOffsets[0] = { 0, 0, 0 };
		blit.srcOffsets[1] = { mipWidth, mipHeight, 1 };
		blit.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.srcSubresource.mipLevel = i - 1;
		blit.srcSubresource.baseArrayLayer = 0;
		blit.srcSubresource.layerCount = 1;
		blit.dstOffsets[0] = { 0, 0, 0 };
		blit.dstOffsets[1] = { mipWidth > 1 ? mipWidth / 2 : 1, mipHeight > 1 ? mipHeight / 2 : 1, 1 };
		blit.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		blit.dstSubresource.mipLevel = i;
		blit.dstSubresource.baseArrayLayer = 0;
		blit.dstSubresource.layerCount = 1;

		vkCmdBlitImage(commandBuffer,
					   _image, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
					   _image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
					   1, &blit,
					   VK_FILTER_LINEAR);

		barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
		barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

		vkCmdPipelineBarrier(commandBuffer,
							 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
							 0, nullptr,
							 0, nullptr,
							 1, &barrier);

		if (mipWidth > 1) mipWidth /= 2;
		if (mipHeight > 1) mipHeight /= 2;
	}

	barrier.subresourceRange.baseMipLevel = _mipLevels - 1;
	barrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
	barrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
	barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

	vkCmdPipelineBarrier(commandBuffer,
						 VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0,
						 0, nullptr,
						 0, nullptr,
						 1, &barrier);

	endSingle(vkDevice,_queue,_pool,commandBuffer);

	_layout = TextureLayout::ShaderReadOnlyOptimal;
}

vk::Image TextureVK::getImage() {
	return _image;
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
