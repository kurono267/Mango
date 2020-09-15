//
// Created by kurono267 on 06.07.18.
//

#include "BufferVK.hpp"
#include <iostream>
#include "../api/Instance.hpp"

using namespace mango::vulkan;

void BufferVK::create(const BufferType &type,const MemoryType& memory,const size_t &size,void* data) {
	_memoryType = memory;
	std::cout << "BufferVK::create" << std::endl;
	_size = size;
	vk::BufferUsageFlags usage;
	if((uint32_t)type & (uint32_t)BufferType::Vertex){
		usage |= vk::BufferUsageFlagBits::eVertexBuffer;
	}
	if((uint32_t)type & (uint32_t)BufferType::Index){
		usage |= vk::BufferUsageFlagBits::eIndexBuffer;
	}
	if((uint32_t)type & (uint32_t)BufferType::Uniform){
		usage |= vk::BufferUsageFlagBits::eUniformBuffer;
	}
	if((uint32_t)type & (uint32_t)BufferType::Storage) {
		usage |= vk::BufferUsageFlagBits::eStorageBuffer;
	}
	if(memory == MemoryType::HOST){
		createBuffer(size,
					 vk::BufferUsageFlagBits::eTransferSrc | usage,
					 vk::MemoryPropertyFlagBits::eHostVisible |
					 vk::MemoryPropertyFlagBits::eHostCoherent,
					 _buffer,_memory);
		if(data)set(data,size,_memory);
	} else if(memory == MemoryType::DEVICE){
		createBuffer(size,
					 vk::BufferUsageFlagBits::eTransferDst | usage,
					 vk::MemoryPropertyFlagBits::eDeviceLocal,
					 _buffer,_memory);
	} else if(memory == MemoryType::DEVICE_HOST) {
		createBuffer(size,
			   		usage,
			   		vk::MemoryPropertyFlagBits::eDeviceLocal | vk::MemoryPropertyFlagBits::eHostVisible,
			   		_buffer, _memory);
		if(data)set(data,size,_memory);
	}
}

void BufferVK::copy(const spBuffer& dst, const spCommandBuffer& cmd){
	auto vkBuffer = std::dynamic_pointer_cast<BufferVK>(dst);
	copy(_buffer,vkBuffer->_buffer,_size,cmd);
}

void BufferVK::set(const void* src,const size_t& size,const vk::DeviceMemory& dst){
	void* map_data = map();
		memcpy(map_data,src,size);
	unmap();
}

void BufferVK::set(const size_t &size, const void *data) {
	set(data,size,_memory);
}

void BufferVK::createBuffer(vk::DeviceSize size,vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
							vk::Buffer& buffer,vk::DeviceMemory& memory){
	vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(),size,usage);
	auto impDevice = Instance::device<DeviceVK>();
	auto vkDevice = impDevice->getDevice();

    buffer = vkDevice.createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = vkDevice.getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo(memRequirements.size,findMemoryType(impDevice->getPhysicalDevice(), memRequirements.memoryTypeBits, properties));
    memory = vkDevice.allocateMemory(allocInfo);

	vkDevice.bindBufferMemory(buffer,memory, 0);
}

void BufferVK::copy(const vk::Buffer& src,const vk::Buffer& dst,const size_t& size, const spCommandBuffer& cmd){
	auto impDevice = Instance::device<DeviceVK>();
	vk::CommandBuffer commands;
	if(!cmd)commands = beginSingle(impDevice->getDevice(),impDevice->getCommandPool());
	else commands = std::static_pointer_cast<CommandBufferVK>(cmd)->getVK();
		commands.copyBuffer(src,dst,vk::BufferCopy(0,0,size));
	if(!cmd)endSingle(impDevice->getDevice(),impDevice->getGraphicsQueue(),impDevice->getCommandPool(),commands);
}

BufferVK::~BufferVK() {
	std::cout << "~BufferVK" << std::endl;
	auto impDevice = Instance::device<DeviceVK>();
	auto vkDevice = impDevice->getDevice();
	if(_mappedData)vkDevice.unmapMemory(_memory);
	if(_memory)vkDevice.freeMemory(_memory);
	if(_buffer)vkDevice.destroyBuffer(_buffer);
}

vk::Buffer BufferVK::getVKBuffer() {
	return _buffer;
}

size_t BufferVK::size() {
	return _size;
}

void *BufferVK::map() {
	if(_memoryType == MemoryType::DEVICE)return nullptr;
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	if(!_mappedData){
		_mappedData = vkDevice.mapMemory(_memory,0,(vk::DeviceSize)_size);
	}
	/*if(_memoryType == MemoryType::DEVICE_HOST){
		vkDevice.invalidateMappedMemoryRanges(vk::MappedMemoryRange(_memory,0,_size));
	}*/
	return _mappedData;
}

void BufferVK::unmap() {
	if(_memoryType == MemoryType::DEVICE || _memoryType == MemoryType::HOST)return;
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vkDevice.flushMappedMemoryRanges(vk::MappedMemoryRange(_memory,0,_size));
}

uint32_t mango::vulkan::findMemoryType(vk::PhysicalDevice pDevice,
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

vk::CommandBuffer mango::vulkan::beginSingle(vk::Device device,vk::CommandPool pool){
	vk::CommandBufferAllocateInfo allocInfo(pool,vk::CommandBufferLevel::ePrimary,1);
	auto cmdBuffers = device.allocateCommandBuffers(allocInfo);

	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	cmdBuffers[0].begin(beginInfo);
	return cmdBuffers[0];
}

void mango::vulkan::endSingle(vk::Device device,vk::Queue queue,vk::CommandPool pool,vk::CommandBuffer commands){
	commands.end();

	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&commands);

	queue.submit(submitInfo,vk::Fence());
	queue.waitIdle();

	device.freeCommandBuffers(pool,{commands});
}
