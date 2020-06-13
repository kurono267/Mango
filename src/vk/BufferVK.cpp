//
// Created by kurono267 on 06.07.18.
//

#include "BufferVK.hpp"
#include <iostream>
#include "../api/Instance.hpp"

using namespace mango::vulkan;

void BufferVK::create(const BufferType &type,const MemoryType& memory,const size_t &size,void* data) {
	std::cout << "BufferVK::create" << std::endl;
	_size = size;
	if(memory == MemoryType::HOST){
		createBuffer(size,
					 vk::BufferUsageFlagBits::eTransferSrc,
					 vk::MemoryPropertyFlagBits::eHostVisible |
					 vk::MemoryPropertyFlagBits::eHostCoherent,
					 _buffer,_memory);
		if(data)set(data,size,_memory);
	} else if(memory == MemoryType::DEVICE){
		vk::BufferUsageFlags gpuUsage;
		if((uint32_t)type & (uint32_t)BufferType::Vertex){
			gpuUsage |= vk::BufferUsageFlagBits::eVertexBuffer;
		}
		if((uint32_t)type & (uint32_t)BufferType::Index){
			gpuUsage |= vk::BufferUsageFlagBits::eIndexBuffer;
		}
		if((uint32_t)type & (uint32_t)BufferType::Uniform){
			gpuUsage |= vk::BufferUsageFlagBits::eUniformBuffer;
		}
		if((uint32_t)type & (uint32_t)BufferType::Storage) {
			gpuUsage |= vk::BufferUsageFlagBits::eStorageBuffer;
		}

		createBuffer(size,
					 vk::BufferUsageFlagBits::eTransferDst | gpuUsage,
					 vk::MemoryPropertyFlagBits::eDeviceLocal,
					 _buffer,_memory);
	}
}

void BufferVK::copy(const spBuffer& dst){
	auto vkBuffer = std::dynamic_pointer_cast<BufferVK>(dst);
	copy(_buffer,vkBuffer->_buffer,_size);
}

void BufferVK::set(const void* src,const size_t& size,const vk::DeviceMemory& dst){
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	void* map_data = vkDevice.mapMemory(dst,0,(vk::DeviceSize)size);
		memcpy(map_data,src,size);
	vkDevice.unmapMemory(dst);
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

void BufferVK::copy(const vk::Buffer& src,const vk::Buffer& dst,const size_t& size){
	auto impDevice = Instance::device<DeviceVK>();
	auto commands = beginSingle(impDevice->getDevice(),impDevice->getCommandPool());
		commands.copyBuffer(src,dst,vk::BufferCopy(0,0,size));
	endSingle(impDevice->getDevice(),impDevice->getGraphicsQueue(),impDevice->getCommandPool(),commands);
}

BufferVK::~BufferVK() {
	std::cout << "~BufferVK" << std::endl;
	auto impDevice = Instance::device<DeviceVK>();
	auto vkDevice = impDevice->getDevice();
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
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	return vkDevice.mapMemory(_memory,0,(vk::DeviceSize)_size);
}

void BufferVK::unmap() {
	auto vkDevice = Instance::device<DeviceVK>()->getDevice();
	vkDevice.unmapMemory(_memory);
}

uint32_t mango::vulkan::findMemoryType(vk::PhysicalDevice pDevice,uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
	vk::PhysicalDeviceMemoryProperties memProperties = pDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

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
