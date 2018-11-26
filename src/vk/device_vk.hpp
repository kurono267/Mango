//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include <api/device.hpp>
#include <api/texture.hpp>
#include "swapchain.hpp"
#include "commandbuffer_vk.hpp"

namespace mango::vulkan {

class RenderPassVK;

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
		DeviceVK() = default;
		~DeviceVK() final;

		std::string device_name() final;

		vk::Device getDevice();
		vk::PhysicalDevice getPhysicalDevice();
		vk::CommandPool getCommandPool();
		vk::Queue getGraphicsQueue();

		spRenderPass createRenderPass() final;
		spPipeline createPipeline(const RenderPattern& rp) final;
		spBuffer createBuffer(const BufferType& type,const size_t& size,void* data) final;

		spTexture createTexture(int width, int height, int miplevels,
								const Format &format, const TextureType &type, const void *data) final;

		Format getDepthFormat() final;

		std::vector<spFramebuffer> getScreenbuffers() final;
		spRenderPass getScreenRenderPass() final;

		spFramebuffer createFramebuffer() final;
		spCommandBuffer createCommandBuffer() final;
		spSemaphore createSemaphore() final;

		void submit(const spCommandBuffer& cmd, const spSemaphore& waitForIt, const spSemaphore& result) final;
		void present(uint32_t screen, const spSemaphore& signal) final;

		uint32_t nextScreen(const spSemaphore& signal);
private:
		void create(const vk::Instance& instance,const vk::SurfaceKHR& surface,const glm::ivec2& size);

		void pickPhysicalDevice();
		bool isDeviceSuitable(const vk::PhysicalDevice& device);

		void createLogicalDevice();

		void createScreen();

		vk::PhysicalDevice _pDevice;
		vk::Device         _device;
		vk::Instance       _instance;
		vk::Queue          _presentQueue;
		vk::Queue          _graphicsQueue;
		vk::Queue          _computeQueue;
		vk::SurfaceKHR     _surface;

		std::shared_ptr<RenderPassVK> _screenRenderPass;
		std::vector<spFramebuffer> _screenbuffers;

		vk::CommandPool    _pool;

		Swapchain          _swapchain;

		glm::ivec2         _size;
};

typedef std::shared_ptr<DeviceVK> spDeviceVK;

class SemaphoreVK : public mango::Semaphore {
	public:
		void create(const spDeviceVK& device);

		vk::Semaphore getVK();
	protected:
		vk::Semaphore _semaphore;
};

};
