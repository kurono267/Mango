//
// Created by kurono267 on 12.10.2021.
//

#include "GPU.hpp"

#include "vulkan/VulkanInit.hpp"
#include "metal/MetalInit.hpp"
//#include "vulkan/VulkanRender.hpp"
//#include "vulkan/VulkanGLFW.hpp"
//#include "vulkan/VulkanResources.hpp"

namespace mango {

void GPU::init(GPUApi api, size_t cacheSize) {
	GPU& gpu = GPU::get();
	switch(api){
		case Vulkan:
			gpu._init = std::make_shared<VulkanInit>();
			gpu._init->init();
			//gpu._resources = std::make_shared<VulkanResources>(std::dynamic_pointer_cast<VulkanInit>(gpu._init));
			//gpu._resources->init(cacheSize);
			//gpu._render = std::make_shared<VulkanRender>(std::dynamic_pointer_cast<VulkanResources>(gpu._resources));
			//gpu._window = std::make_shared<VulkanGLFW>(std::dynamic_pointer_cast<VulkanResources>(gpu._resources));
			break;
		case Metal:
			gpu._init = std::make_shared<MetalInit>();
			gpu._init->init();
			break;
		default:
			throw std::runtime_error("Select unsupported API");
	}
}

}
