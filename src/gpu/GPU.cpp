//
// Created by kurono267 on 12.10.2021.
//

#include "GPU.hpp"

// Vulkan
#include "vulkan/VulkanInit.hpp"
#include "vulkan/VulkanResources.hpp"
//#include "vulkan/VulkanRender.hpp"
//#include "vulkan/VulkanGLFW.hpp"
// Metal
#include "metal/MetalInit.hpp"
#include "metal/MetalResources.hpp"

namespace mango {

void GPU::init(GPUApi api) {
	GPU& gpu = GPU::get();
	switch(api){
		case Vulkan:
			gpu._init = std::make_shared<VulkanInit>();
			gpu._init->init();
			gpu._resources = std::make_shared<VulkanResources>(std::static_pointer_cast<VulkanInit>(gpu._init));
			//gpu._render = std::make_shared<VulkanRender>(std::dynamic_pointer_cast<VulkanResources>(gpu._resources));
			//gpu._window = std::make_shared<VulkanGLFW>(std::dynamic_pointer_cast<VulkanResources>(gpu._resources));
			break;
		case Metal:
			gpu._init = std::make_shared<MetalInit>();
			gpu._init->init();
			gpu._resources = std::make_shared<MetalResources>(std::static_pointer_cast<MetalInit>(gpu._init));
			break;
		default:
			throw std::runtime_error("Select unsupported API");
	}
}

}
