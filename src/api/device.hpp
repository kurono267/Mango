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
class RenderPattern;
class Buffer;
class Texture;
class Framebuffer;
class CommandBuffer;

typedef std::shared_ptr<RenderPass> spRenderPass;
typedef std::shared_ptr<Pipeline> spPipeline;
typedef std::shared_ptr<Buffer> spBuffer;
typedef std::shared_ptr<Texture> spTexture;
typedef std::shared_ptr<Framebuffer> spFramebuffer;
typedef std::shared_ptr<CommandBuffer> spCommandBuffer;

class Device {
	public:
		Device() = default;
		virtual ~Device() = default;

		virtual std::string device_name() = 0;

		virtual spRenderPass createRenderPass() = 0;
		virtual spPipeline createPipeline(const RenderPattern& rp) = 0;
		virtual spBuffer createBuffer(const BufferType& type,const size_t& size,void* data = nullptr) = 0;

		virtual spTexture createTexture(const int width,const int height,
							const int miplevels, const Format& format,const TextureType& type, const void* data = nullptr) = 0;

		virtual Format getDepthFormat() = 0;

		virtual std::vector<spFramebuffer> getScreenbuffers() = 0;
		virtual spRenderPass getScreenRenderPass() = 0;

		virtual spFramebuffer createFramebuffer() = 0;
		virtual spCommandBuffer createCommandBuffer() = 0;
};

typedef std::shared_ptr<Device> spDevice;

};

