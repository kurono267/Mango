//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_BUFFER_VK_HPP
#define MANGO_BUFFER_VK_HPP

#include <api/buffer.hpp>
#include "default.hpp"
#include "device_vk.hpp"

namespace mango::vulkan {

class BufferVK : public Buffer {
	public:
		BufferVK() = default;
		~BufferVK() final;

		void create(const spDeviceVK& device,const BufferType &type,const size_t &size,void* data = nullptr);
		void set(const size_t &size, const void *data) override;
	protected:
		void createBuffer(vk::DeviceSize size,vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
							vk::Buffer& buffer,vk::DeviceMemory& memory);

		void copy(const vk::Buffer& src,const vk::Buffer& dst,const size_t& size);
		void set(const void* data,const size_t& size,const vk::DeviceMemory& dst);

		vk::DeviceMemory _cpuMemory;
		vk::Buffer _cpuBuffer;
		vk::DeviceMemory _gpuMemory;
		vk::Buffer _gpuBuffer;

		spDeviceVK _device;
};

uint32_t findMemoryType(vk::PhysicalDevice pDevice,uint32_t typeFilter, vk::MemoryPropertyFlags properties);

vk::CommandBuffer beginSingle(vk::Device device,vk::CommandPool pool);
void              endSingle(vk::Device device,vk::Queue queue,vk::CommandPool pool,vk::CommandBuffer commands);

};

#endif //MANGO_BUFFER_VK_HPP
