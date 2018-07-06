//
// Created by kurono267 on 06.07.18.
//

#include "buffer_vk.hpp"

using namespace mango::vulkan;

void BufferVK::create(const spDeviceVK& device,const BufferType &type,const size_t &size,void* data = nullptr) {
	_device = device;
	createBuffer(size,
		vk::BufferUsageFlagBits::eTransferSrc,
		vk::MemoryPropertyFlagBits::eHostVisible |
		vk::MemoryPropertyFlagBits::eHostCoherent,
		_cpuBuffer,_cpuMemory);
	if(data)set(data,size,_cpuMemory);

	vk::BufferUsageFlagBits gpuUsage;
	switch (type){
		case BufferType::Vertex:
		gpuUsage = vk::BufferUsageFlagBits::eVertexBuffer;
		break;
		case BufferType::Index:
		gpuUsage = vk::BufferUsageFlagBits::eIndexBuffer;
		break;
		case BufferType::Uniform:
		gpuUsage = vk::BufferUsageFlagBits::eUniformBuffer;
		break;
		case BufferType::Storage:
		gpuUsage = vk::BufferUsageFlagBits::eStorageBuffer;
		break;
	}

	createBuffer(size,
		vk::BufferUsageFlagBits::eTransferDst | gpuUsage,
		vk::MemoryPropertyFlagBits::eDeviceLocal,
		_gpuBuffer,_gpuMemory);

	if(data)copy(_cpuBuffer,_gpuBuffer,size);
}

void BufferVK::set(const void* src,const size_t& size,const vk::DeviceMemory& dst){
	void* map_data = _device->getDevice().mapMemory(dst,0,(vk::DeviceSize)size);
		memcpy(map_data,src,size);
	_device->getDevice().unmapMemory(dst);
}

void BufferVK::set(const size_t &size, const void *data) {
	set(data,size,_cpuMemory);
	copy(_cpuBuffer,_gpuBuffer,size);
}

void BufferVK::createBuffer(vk::DeviceSize size,vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
							vk::Buffer& buffer,vk::DeviceMemory& memory){
	vk::BufferCreateInfo bufferInfo(vk::BufferCreateFlags(),size,usage);

    buffer = _device->getDevice().createBuffer(bufferInfo);

    vk::MemoryRequirements memRequirements = _device->getDevice().getBufferMemoryRequirements(buffer);

    vk::MemoryAllocateInfo allocInfo(memRequirements.size,findMemoryType(_device->getPDevice(), memRequirements.memoryTypeBits, properties));
    memory = _device->getDevice().allocateMemory(allocInfo);

    _device->getDevice().bindBufferMemory(buffer,memory, 0);
}

void BufferVK::copy(const vk::Buffer& src,const vk::Buffer& dst,const size_t& size){
	auto commands = beginSingle(_device->getDevice(),_device->getCommandPool());
		commands.copyBuffer(src,dst,vk::BufferCopy(0,0,size));
	endSingle(_device->getDevice(),_device->getGraphicsQueue(),_device->getCommandPool(),commands);
}

BufferVK::~BufferVK() {

}

uint32_t findMemoryType(vk::PhysicalDevice pDevice,uint32_t typeFilter, vk::MemoryPropertyFlags properties) {
	vk::PhysicalDeviceMemoryProperties memProperties = pDevice.getMemoryProperties();

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

vk::CommandBuffer beginSingle(vk::Device device,vk::CommandPool pool){
	vk::CommandBufferAllocateInfo allocInfo(pool,vk::CommandBufferLevel::ePrimary,1);
	auto cmdBuffers = device.allocateCommandBuffers(allocInfo);

	vk::CommandBufferBeginInfo beginInfo(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
	cmdBuffers[0].begin(beginInfo);
	return cmdBuffers[0];
}

void endSingle(vk::Device device,vk::Queue queue,vk::CommandPool pool,vk::CommandBuffer commands){
	commands.end();

	vk::SubmitInfo submitInfo;
	submitInfo.setCommandBufferCount(1);
	submitInfo.setPCommandBuffers(&commands);

	queue.submit(submitInfo,vk::Fence());
	queue.waitIdle();

	device.freeCommandBuffers(pool,{commands});
}
