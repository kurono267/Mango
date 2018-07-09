//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include <api/device.hpp>
#include <api/texture.hpp>
#include "swapchain.hpp"

namespace mango::vulkan {

#ifdef NDEBUG
const bool enableValidationLayers = true;
#else
const bool enableValidationLayers = true;
#endif

const std::vector<const char*> validationLayers = {
    "VK_LAYER_LUNARG_standard_validation",
    "VK_LAYER_LUNARG_parameter_validation"
};

class DeviceVK : public Device, public std::enable_shared_from_this<DeviceVK> {
	friend class InstanceVK;
	public:
		DeviceVK();
		~DeviceVK() final;

		std::string device_name() final;

		vk::Device getDevice();
		vk::PhysicalDevice getPhysicalDevice();
		vk::CommandPool getCommandPool();
		vk::Queue getGraphicsQueue();

		spRenderPass createRenderPass() final;
		spPipeline createPipeline(const RenderPattern& rp) final;
		spBuffer createBuffer(const BufferType& type,const size_t& size,void* data = nullptr) final;

		spTexture createTexture(const int width, const int height, const int miplevels,
								const Format &format, const TextureType &type, const void *data) final;

		Format getDepthFormat() final;

		spFramebuffer createFramebuffer() final;
private:
		void create(const vk::Instance& instance,const vk::SurfaceKHR& surface,const glm::ivec2& size);

		void pickPhysicalDevice();
		bool isDeviceSuitable(const vk::PhysicalDevice& device);

		void createLogicalDevice();

		vk::PhysicalDevice _pDevice;
		vk::Device         _device;
		vk::Instance       _instance;
		vk::Queue          _presentQueue;
		vk::Queue          _graphicsQueue;
		vk::Queue          _computeQueue;
		vk::SurfaceKHR     _surface;

		vk::CommandPool    _pool;

		Swapchain          _swapchain;

		glm::ivec2         _size;
};

typedef std::shared_ptr<DeviceVK> spDeviceVK;

};
