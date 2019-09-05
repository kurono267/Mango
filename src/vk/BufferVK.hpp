//
// Created by kurono267 on 06.07.18.
//

#ifndef MANGO_BUFFERVK_HPP
#define MANGO_BUFFERVK_HPP

#include <api/Buffer.hpp>
#include "default.hpp"
#include "DeviceVK.hpp"

namespace mango::vulkan {

class BufferVK : public Buffer {
	public:
		BufferVK() = default;
		virtual ~BufferVK() final;

		void create(const BufferType &type,const MemoryType& memory,const size_t &size,void* data = nullptr);
		void set(const size_t &size, const void *data) final;

		void copy(const spBuffer& dst) final;

		size_t size() final;
		void* map() final;
		void unmap() final;

		vk::Buffer getVKBuffer();
	protected:
		void createBuffer(vk::DeviceSize size,vk::BufferUsageFlags usage, vk::MemoryPropertyFlags properties,
							vk::Buffer& buffer,vk::DeviceMemory& memory);

		void copy(const vk::Buffer& src,const vk::Buffer& dst,const size_t& size);
		void set(const void* data,const size_t& size,const vk::DeviceMemory& dst);

		vk::DeviceMemory _memory;
		vk::Buffer _buffer;

		size_t _size;
};

uint32_t findMemoryType(vk::PhysicalDevice pDevice,uint32_t typeFilter, vk::MemoryPropertyFlags properties);

vk::CommandBuffer beginSingle(vk::Device device,vk::CommandPool pool);
void              endSingle(vk::Device device,vk::Queue queue,vk::CommandPool pool,vk::CommandBuffer commands);

};

#endif //MANGO_BUFFERVK_HPP
