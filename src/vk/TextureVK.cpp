//
// Created by kurono267 on 06.07.18.
//

#include "TextureVK.hpp"
#include "BufferVK.hpp"

using namespace mango::vulkan;

TextureViewVK::~TextureViewVK(){

}

vk::ImageView TextureViewVK::getView(){
	return _view;
}

TextureVK::~TextureVK() {

}

void TextureVK::create(const spDevice& device,const int width,const int height,const int miplevels,const Format& format,const mango::TextureType &type, const vk::Image& image){
    auto internal = std::dynamic_pointer_cast<DeviceVK>(device);

    _vk_device = internal->getDevice();
    _pool = internal->getCommandPool();
    _queue = internal->getGraphicsQueue();
    _image = image;
    _width = width;
    _height = height;
    _format = format;
    _mipLevels = miplevels;
    _type = type;
}

void TextureVK::create(const spDevice& device,const int width, const int height,const int miplevels , const Format& format, const mango::TextureType &type) {
	_width = width;
	_height = height;
	_mipLevels = miplevels;
	_type = type;
	_format = format;

	vk::ImageUsageFlags usage;
	vk::ImageLayout layout = vk::ImageLayout::ePreinitialized;
	switch(type){
		case mango::TextureType::Input:
		usage = vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled;
		break;
		case mango::TextureType::Output:
		usage = vk::ImageUsageFlagBits::eColorAttachment;
		break;
		case mango::TextureType::DepthStencil:
		usage = vk::ImageUsageFlagBits::eDepthStencilAttachment;
		layout = vk::ImageLayout::eUndefined;
		break;
	}

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
	_pool = mango->getCommandPool();
	_queue = mango->getGraphicsQueue();

	// Create Image
	_image = vk_device.createImage(imageInfo);
	auto memoryReq =vk_device.getImageMemoryRequirements(_image);

	vk::MemoryPropertyFlags memory_type = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vk::MemoryAllocateInfo allocInfo(memoryReq.size,findMemoryType(mango->getPhysicalDevice(),memoryReq.memoryTypeBits, memory_type));
	_memory = vk_device.allocateMemory(allocInfo);

	vk_device.bindImageMemory(_image,_memory,0);
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
	vk::CommandBuffer commandBuffer = beginSingle(_vk_device,_pool);

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

	endSingle(_vk_device,_queue,_pool,commandBuffer);
	_layout = newLayout;
}

mango::spTextureView TextureVK::createTextureView(const ComponentMapping& componentMapping,const int minLevel,const int maxLevel){
	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(_format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;
	auto viewCreateInfo = vk::ImageViewCreateInfo(
		vk::ImageViewCreateFlags(),
		_image,
		vk::ImageViewType::e2D,
		formatVK(_format),
		componentMappingVK(componentMapping),
		vk::ImageSubresourceRange(
			imageAspectFlags,
			minLevel, maxLevel!=-1?maxLevel:_mipLevels, 0, 1)
	);
	auto texView = std::make_shared<TextureViewVK>(shared_from_this());
	texView->_view = _vk_device.createImageView(viewCreateInfo);
	texView->_isInit = true;
	return texView;
}

void TextureVK::set(const mango::spBuffer &buffer) {
	transition(vk::ImageLayout::eTransferDstOptimal);
    setBuffer(buffer,glm::ivec2(_width,_height),0,0,0);
    transition(vk::ImageLayout::eShaderReadOnlyOptimal);
}

void TextureVK::setBuffer(const spBuffer& buffer, const glm::ivec2& size, const uint& mipLevel, const uint& layer, const uint& offsetBuffer){
    vk::CommandBuffer commandBuffer = beginSingle(_vk_device,_pool);

    vk::ImageSubresourceLayers subRes(
            vk::ImageAspectFlagBits::eColor,
            mipLevel, /* Mip levels current*/ layer, 1 /* Layers current and count */ );

    vk::BufferImageCopy region(
            /* bufferOffset bufferRowLength bufferImageHeight */
            offsetBuffer, 0, 0,
            /* subRes imageOffest */
            subRes, {0, 0, 0},
            /* imageExtent */
            {(uint)size.x,(uint)size.y,1}
    );

    commandBuffer.copyBufferToImage(std::dynamic_pointer_cast<BufferVK>(buffer)->getVKBuffer(),_image,vk::ImageLayout::eTransferDstOptimal, {region});

    endSingle(_vk_device,_queue,_pool,commandBuffer);
}

vk::Sampler mango::vulkan::createSampler(const mango::spDevice &device, const mango::Sampler &sampler) {
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
	return std::dynamic_pointer_cast<DeviceVK>(device)->getDevice().createSampler(createInfo);
}
