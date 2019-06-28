//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include "types.hpp"

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

class Device {
	public:
		Device() = default;
		virtual ~Device() = default;

		virtual std::string device_name() = 0;

		virtual spRenderPass createRenderPass() = 0;
		virtual spPipeline createPipeline(const PipelineInfo& rp) = 0;
		virtual spBuffer createBuffer(const BufferType& type,const MemoryType& memory,const size_t& size,void* data = nullptr) = 0;
		virtual spDescSet createDescSet() = 0;

		virtual spTexture createTexture(int width,int height,
					int miplevels, const Format& format,const TextureType& type) = 0;

		virtual Format getDepthFormat() = 0;

		virtual std::vector<spFramebuffer> getScreenbuffers() = 0;
		virtual spRenderPass getScreenRenderPass() = 0;

		virtual spFramebuffer createFramebuffer() = 0;
		virtual spCommandBuffer createCommandBuffer() = 0;
		virtual spSemaphore createSemaphore() = 0;

		virtual void submit(const spCommandBuffer& cmd, const spSemaphore& waitForIt, const spSemaphore& result) = 0;
		virtual void present(uint32_t screen, const spSemaphore& signal) = 0;

		virtual uint32_t nextScreen(const spSemaphore& signal) = 0;

		virtual void release() = 0;
		virtual void waitIdle() = 0;
};

typedef std::shared_ptr<Device> spDevice;

};

