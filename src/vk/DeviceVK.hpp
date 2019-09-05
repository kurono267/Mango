//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include <api/Device.hpp>
#include <api/Texture.hpp>
#include "Swapchain.hpp"
#include "CommandBufferVK.hpp"

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

class DeviceVK : public Device {
	friend class InstanceVK;
	public:
		DeviceVK() = default;
		~DeviceVK() override;

		std::string deviceName() final;

		vk::Device getDevice();
		vk::PhysicalDevice getPhysicalDevice();
		vk::CommandPool getCommandPool();
		vk::CommandPool getComputeCommandPool();
		vk::Queue getGraphicsQueue();
		vk::Queue getComputeQueue();

		spRenderPass createRenderPass() final;
		spPipeline createPipeline(const PipelineInfo& rp) final;

	    spDescSet createDescSet() final;
	    spBuffer createBuffer(const BufferType& type,const MemoryType& memory,const size_t& size,void* data) final;

		spTexture createTexture(int width, int height, int miplevels,
								const Format &format, const TextureType &type) final;

		spFramebuffer createFramebuffer() final;
		spCommandBuffer createCommandBuffer() final;
		spSemaphore createSemaphore() final;

		spCompute createCompute(const std::string& filename, const std::vector<spDescSet>& descSets) final;

		Format getDepthFormat() final;

		std::vector<spFramebuffer> getScreenbuffers() final;
		spRenderPass getScreenRenderPass() final;

		void submit(const spCommandBuffer& cmd, const spSemaphore& waitForIt, const spSemaphore& result) final;
		void present(uint32_t screen, const spSemaphore& signal) final;

		uint32_t nextScreen(const spSemaphore& signal) final;

	    void waitIdle() final;
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
		vk::CommandPool    _computePool;

		Swapchain          _swapchain;

		glm::ivec2         _size;
};

typedef std::shared_ptr<DeviceVK> spDeviceVK;

class SemaphoreVK : public mango::Semaphore {
	public:
		SemaphoreVK();

		vk::Semaphore getVK();
	protected:
		vk::Semaphore _semaphore;
};

typedef std::shared_ptr<SemaphoreVK> spSemaphoreVK;

};
