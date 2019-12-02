//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include "Types.hpp"

#include <vector>

namespace mango {

class RenderPass;
class Pipeline;
class PipelineInfo;
class Buffer;
class Texture;
class Framebuffer;
class CommandBuffer;
class DescSet;
class Compute;
class RenderTarget;

// Semaphore empty class
class Semaphore {
	public:
		Semaphore() = default;
		virtual ~Semaphore() = default;
};

typedef std::shared_ptr<RenderPass> spRenderPass;
typedef std::shared_ptr<Pipeline> spPipeline;
typedef std::shared_ptr<Buffer> spBuffer;
typedef std::shared_ptr<Texture> spTexture;
typedef std::shared_ptr<Framebuffer> spFramebuffer;
typedef std::shared_ptr<CommandBuffer> spCommandBuffer;
typedef std::shared_ptr<Semaphore> spSemaphore;
typedef std::shared_ptr<DescSet> spDescSet;
typedef std::shared_ptr<Compute> spCompute;
typedef std::shared_ptr<RenderTarget> spRenderTarget;

/// Abstract Device API
class Device {
	public:
		Device() = default;
		virtual ~Device() = default;

		virtual void release() = 0;
		/// Get Device Name
		/// @return device name
		virtual std::string deviceName() = 0;

		/// Get Default Depth Format
		/// @return default depth format
		virtual Format getDepthFormat() = 0;

		virtual spRenderPass createRenderPass() = 0;
		virtual spPipeline createPipeline(const PipelineInfo& rp) = 0;
		virtual spBuffer createBuffer(const BufferType& type,const MemoryType& memory,const size_t& size,void* data = nullptr) = 0;
		virtual spDescSet createDescSet() = 0;
		virtual spCommandBuffer createCommandBuffer() = 0;
		virtual spSemaphore createSemaphore() = 0;
		virtual spFramebuffer createFramebuffer(const glm::ivec2& size) = 0;
		virtual spFramebuffer createFramebuffer(const int width, const int height) = 0;
		virtual spCompute createCompute(const std::string& filename, const std::vector<spDescSet>& descSets) = 0;

		virtual spTexture createTexture(int width,int height,
										int miplevels, const Format& format,const TextureType& type) = 0;

		virtual glm::ivec2 getScreenSize() = 0;

		/// Get Screen render targets
		/// @return vector of screen render target
		virtual std::vector<spRenderTarget> getScreenRenderTargets() = 0;

		/// Submit command buffer
		/// @param cmd command buffer
		/// @param waitForIt semaphore waited before execute command buffer
		/// @param result semaphore changed when execute complete
		virtual void submit(const spCommandBuffer& cmd, const spSemaphore& waitForIt, const spSemaphore& result) = 0;
		/// Present screen
		/// @param screen Screen ID
		/// @param signal semaphore changed when execute complete
		virtual void present(uint32_t screen, const spSemaphore& signal) = 0;

		/// Get next screen ID
		/// @param semaphore changed when screen available
		/// @return screen ID
		virtual uint32_t nextScreen(const spSemaphore& signal) = 0;

		/// Wait when all executed command buffer finished
		virtual void waitIdle() = 0;
};

typedef std::shared_ptr<Device> spDevice;

};

