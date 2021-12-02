//
// Created by kurono267 on 12.10.2021.
//

#pragma once

#include <cstddef>
#include <memory>
#include "GPUInit.hpp"
//#include "gpu/GPURender.hpp"
//#include "gpu/GPUResources.hpp"
//#include "gpu/GPUWindow.hpp"

namespace mango {

enum GPUApi {
	Vulkan,
	Metal
};

class GPU {
	public:
		static GPU& get() {
			static GPU gpu;
			return gpu;
		}

		// GPU Init
		static void init(GPUApi api);

		// GPU Resources
		/*static ResourceID createTexture(const TextureLayout &layout) {
			return GPU::get()._resources->createTexture(layout);
		}

		static ResourceID createBuffer(const BufferType &type, size_t sizeInBytes) {
			return GPU::get()._resources->createBuffer(type,sizeInBytes);
		}

		static GPUTaskID store(const ResourceID &id, const spBufferRaw &raw) {
			return GPU::get()._resources->store(id,raw);
		}
		static GPUTaskID load(const ResourceID &id, const spBufferRaw &raw) {
			return GPU::get()._resources->load(id,raw);
		}

		static void waitAsync(GPUTaskID task, const std::function<void()>& function) {
			GPU::get()._resources->waitAsync(task,function);
		}

		static void updateResources() {
			GPU::get()._resources->update();
		}
		static void waitResources() {
			GPU::get()._resources->waitAll();
		}

		// GPU Window
		static SwapChainID createSwapChain(const spMainApp &mainApp) {
			return GPU::get()._window->createSwapChain(mainApp);
		}

		static std::vector<ResourceID> getSwapChainTextures(SwapChainID id) {
			return GPU::get()._window->getSwapChainTextures(id);
		}

		// GPU Render
		static RenderPassID addRenderPass(const RenderPipeline &renderPass) {
			return GPU::get()._render->addRenderPass(renderPass);
		}

		static void render(const RenderPassID& id) {
			GPU::get()._render->render(id);
		}
		static void waitAll() {
			GPU::get()._render->waitAll();
		}*/

		std::shared_ptr<GPUInit> _init;
		//std::shared_ptr<GPUResources> _resources;
		//std::shared_ptr<GPUWindow> _window;
		//std::shared_ptr<GPURender> _render;
};

}