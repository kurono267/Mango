//
// Created by kurono267 on 18.06.2021.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <queue>

#include "../GPUResources.hpp"

#include "VulkanInit.hpp"

using namespace mango;

struct BufferVK {
	BufferType type;
	BufferMemory memoryType;

	vk::DeviceMemory memory;
	vk::Buffer buffer;
};

struct TextureVK {
	TextureDesc desc;

	vk::DeviceMemory memory;
	vk::ImageLayout layout = vk::ImageLayout::eUndefined;
	vk::Image image;
	vk::ImageView view;
};

class VulkanResources : public GPUResources {
	public:
		explicit VulkanResources(const std::shared_ptr<VulkanInit>& vk);

		TextureID createTexture(const TextureDesc &desc) override;
		TextureID createTexture(const TextureDesc& desc, const vk::Image& image);

		BufferID createBuffer(BufferType type, BufferMemory memory, size_t sizeInBytes) override;

		std::shared_ptr<VulkanInit> vk;
	private:
		std::vector<BufferVK> _buffers;
		std::vector<TextureVK> _textures;
	private:
		void transitionTexture(vk::CommandBuffer& cmd,const TextureID& textureID,const vk::ImageLayout& to);
};
