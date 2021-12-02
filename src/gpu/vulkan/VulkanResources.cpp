//
// Created by kurono267 on 18.06.2021.
//

#include "VulkanResources.hpp"
#include <iostream>
#include "utils/VkTextureTypes.hpp"

uint32_t findMemoryType(vk::PhysicalDevice pDevice,
						uint32_t memoryTypeBitsRequirement,
						vk::MemoryPropertyFlags requiredProperties) {
	vk::PhysicalDeviceMemoryProperties pMemoryProperties = pDevice.getMemoryProperties();

	const uint32_t memoryCount = pMemoryProperties.memoryTypeCount;
	for (uint32_t memoryIndex = 0; memoryIndex < memoryCount; ++memoryIndex) {
		const uint32_t memoryTypeBits = (1 << memoryIndex);
		const bool isRequiredMemoryType = memoryTypeBitsRequirement & memoryTypeBits;

		const vk::MemoryPropertyFlags properties =
				pMemoryProperties.memoryTypes[memoryIndex].propertyFlags;
		const bool hasRequiredProperties =
				(properties & requiredProperties) == requiredProperties;

		if (isRequiredMemoryType && hasRequiredProperties)
			return static_cast<uint32_t>(memoryIndex);
	}

	// failed to find memory type
	throw std::runtime_error("failed to find suitable memory type!");
}

VulkanResources::VulkanResources(const std::shared_ptr<VulkanInit>& vk) : vk(vk) {}

TextureID VulkanResources::createTexture(const TextureDesc &desc) {
	TextureVK texture;
	texture.desc = desc;

	vk::ImageType imageType = vk::ImageType::e2D;
	vk::ImageViewType viewType = vk::ImageViewType::e2D;
	if(desc.type == TextureType::Texture3D){
		imageType = vk::ImageType::e3D;
		viewType = vk::ImageViewType::e3D;
	}

	vk::Extent3D extent3D(desc.width,desc.height,desc.depth);
	vk::Format format = formatVK(desc.format);

	vk::ImageUsageFlagBits attachmentType = hasDepthComponent(desc.format)?vk::ImageUsageFlagBits::eDepthStencilAttachment:vk::ImageUsageFlagBits::eColorAttachment;
	vk::ImageUsageFlags usage = vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | attachmentType | vk::ImageUsageFlagBits::eSampled;

	vk::ImageCreateInfo imageInfo(
			vk::ImageCreateFlags(), // Basic
			imageType, // Type 1D,2D,3D
			format, // Format
			extent3D, // Width, Height and Depth
			desc.mipLevels, // Mip Levels
			desc.layers, // Array Layers
			vk::SampleCountFlagBits::e1, // Samples
			vk::ImageTiling::eOptimal,
			usage,
			vk::SharingMode::eExclusive, 0, nullptr, vk::ImageLayout::eUndefined
	);

	// Create Image
	texture.image = vk->device.createImage(imageInfo);
	auto memoryReq = vk->device.getImageMemoryRequirements(texture.image);

	vk::MemoryPropertyFlags memory_type = vk::MemoryPropertyFlagBits::eDeviceLocal;
	vk::MemoryAllocateInfo allocInfo(memoryReq.size,findMemoryType(vk->pDevice,memoryReq.memoryTypeBits, memory_type));
	texture.memory = vk->device.allocateMemory(allocInfo);

	vk->device.bindImageMemory(texture.image,texture.memory,0);

	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(desc.format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;

	vk::ComponentMapping componentMapping;

	auto viewCreateInfo = vk::ImageViewCreateInfo(
			vk::ImageViewCreateFlags(),
			texture.image,
			viewType,
			format,
			componentMapping,
			vk::ImageSubresourceRange(
					imageAspectFlags,
					0, desc.mipLevels, 0, desc.layers)
	);
	texture.view = vk->device.createImageView(viewCreateInfo);

	_textures.push_back(texture);
	return (TextureID)(_textures.size()-1);
}

TextureID VulkanResources::createTexture(const TextureDesc& desc, const vk::Image& image) {
	TextureVK texture;
	texture.desc = desc;
	texture.image = image;

	vk::ImageAspectFlags imageAspectFlags = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(desc.format))imageAspectFlags = vk::ImageAspectFlagBits::eDepth;

	vk::ComponentMapping componentMapping;

	auto viewCreateInfo = vk::ImageViewCreateInfo(
			vk::ImageViewCreateFlags(),
			image,
			vk::ImageViewType::e2D,
			formatVK(desc.format),
			componentMapping,
			vk::ImageSubresourceRange(
					imageAspectFlags,
					0, desc.mipLevels, 0, desc.layers)
					);
	texture.view = vk->device.createImageView(viewCreateInfo);
	texture.layout = vk::ImageLayout::ePreinitialized;

	_textures.push_back(texture);
	return (TextureID)(_textures.size()-1);
}

BufferID VulkanResources::createBuffer(BufferType type, BufferMemory memoryType, size_t sizeInBytes) {
	BufferVK buffer;
	buffer.type = type;
	buffer.memoryType = memoryType;

	vk::BufferUsageFlags usage = vk::BufferUsageFlagBits::eTransferDst | vk::BufferUsageFlagBits::eTransferSrc;
	if((uint16_t)type & (uint16_t)BufferType::VertexBuffer){
		usage |= vk::BufferUsageFlagBits::eVertexBuffer;
	}
	if((uint16_t)type & (uint16_t)BufferType::IndexBuffer){
		usage |= vk::BufferUsageFlagBits::eIndexBuffer;
	}
	if((uint16_t)type & (uint16_t)BufferType::Storage){
		usage |= vk::BufferUsageFlagBits::eStorageBuffer;
	}
	if((uint16_t)type & (uint16_t)BufferType::Uniform) {
		usage |= vk::BufferUsageFlagBits::eUniformBuffer;
	}

	vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(),sizeInBytes,usage);

	buffer.buffer = vk->device.createBuffer(bufferInfo);

	vk::MemoryRequirements memRequirements = vk->device.getBufferMemoryRequirements(buffer.buffer);
	vk::MemoryPropertyFlags memoryProperties;
	switch(memoryType){
		case BufferMemory::CPU:
			memoryProperties = vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible;
			break;
		case BufferMemory::GPU:
			memoryProperties = vk::MemoryPropertyFlagBits::eDeviceLocal;
			break;
	}

	vk::MemoryAllocateInfo allocInfo(memRequirements.size,findMemoryType(vk->pDevice, memRequirements.memoryTypeBits, memoryProperties));
	buffer.memory = vk->device.allocateMemory(allocInfo);

	vk->device.bindBufferMemory(buffer.buffer,buffer.memory, 0);

	_buffers.push_back(buffer);
	return (BufferID)(_buffers.size()-1);
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

void VulkanResources::transitionTexture(vk::CommandBuffer& cmd,const TextureID& textureID,const vk::ImageLayout& to) {
	auto texture = _textures[(size_t)textureID];

	vk::ImageAspectFlags aspectMask = vk::ImageAspectFlagBits::eColor;
	if(hasDepthComponent(texture.desc.format)){
		aspectMask = vk::ImageAspectFlagBits::eDepth;
		if (hasStencilComponent(texture.desc.format)) {
			aspectMask |= vk::ImageAspectFlagBits::eStencil;
		}
	}

	vk::AccessFlags srcAccess;
	vk::AccessFlags dstAccess;
	vk::PipelineStageFlags srcStage;
	vk::PipelineStageFlags dstStage;
	flagsFromLayout(texture.layout,srcAccess,srcStage);
	flagsFromLayout(to,dstAccess,dstStage);

	vk::ImageSubresourceRange subRes(
			aspectMask,
			0, texture.desc.mipLevels, /* Mip levels current and count*/ 0, texture.desc.layers /* Layers current and count */ );

	vk::ImageMemoryBarrier barrier(
			srcAccess,
			dstAccess,
			texture.layout,
			to,
			VK_QUEUE_FAMILY_IGNORED,
			VK_QUEUE_FAMILY_IGNORED,
			texture.image,
			subRes
	);

	cmd.pipelineBarrier(
			srcStage, dstStage,
			vk::DependencyFlagBits::eByRegion,
			0, nullptr,
			0, nullptr,
			1, &barrier
	);

	texture.layout = to;
}