//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include "../api/Device.hpp"
#include "../api/Texture.hpp"
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
    "VK_LAYER_KHRONOS_validation"
};

class DeviceVK : public Device, public std::enable_shared_from_this<DeviceVK> {
	friend class InstanceVK;
	public:
		DeviceVK() = default;
		~DeviceVK() override;

		void release() final;

		std::string deviceName() final;

		vk::Device getDevice();
		vk::PhysicalDevice getPhysicalDevice();
		vk::CommandPool getCommandPool();
		vk::CommandPool getComputeCommandPool();
		vk::Queue getGraphicsQueue();
		vk::Queue getComputeQueue();

		spRenderPass createRenderPass() final;
		spPipeline createPipeline(const PipelineInfo& rp) final;

		spDescLayout createDescLayout() final;
		spDescPool createDescPool(size_t numDescSets, const spDescLayout& layout) final;
	    spBuffer createBuffer(const BufferType& type,const MemoryType& memory,const size_t& size,void* data) final;

		spTexture createTexture(int width, int height, int miplevels,
								const Format &format, const TextureType &type) final;
		spTexture createTexture3D(int width, int height, int depth, int miplevels, const Format& format, const TextureType& type) final;
		spTexture createTexture() final;

		spFramebuffer createFramebuffer(const glm::ivec2& size) final;
		spFramebuffer createFramebuffer(const int width, const int height) final;
		spCommandBuffer createCommandBuffer() final;
		spSemaphore createSemaphore() final;
		spFence createFence(bool status) final;

		glm::ivec2 getScreenSize() final;

		spCompute createCompute(const std::string& filename, const std::vector<spDescSet>& descSets) final;

		Format getDepthFormat() final;

		std::vector<spRenderTarget> getScreenRenderTargets() final;

		void submit(const spCommandBuffer& cmd, const spSemaphore& waitForIt, const spSemaphore& result, const mango::spFence& fence = nullptr) final;
		void present(uint32_t screen, const spSemaphore& signal) final;

		uint32_t nextScreen(const spSemaphore& signal,const spFence& fence) final;

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

		std::vector<spRenderTarget> _screenRenderTargets;

		vk::CommandPool    _pool;
		vk::CommandPool    _computePool;

		std::unique_ptr<Swapchain> _swapchain;

		glm::ivec2         _size;
};

typedef std::shared_ptr<DeviceVK> spDeviceVK;

class SemaphoreVK : public mango::Semaphore {
	public:
		SemaphoreVK();
		~SemaphoreVK();

		vk::Semaphore getVK();
	protected:
		vk::Semaphore _semaphore;
};

typedef std::shared_ptr<SemaphoreVK> spSemaphoreVK;

class FenceVK : public mango::Fence {
	public:
		FenceVK(bool status);
		~FenceVK();

		vk::Fence& getVK();

		void reset() final;
		void wait(uint64_t timeout) final;

		bool status() final;
	protected:
		vk::Fence _fence;
};

};
