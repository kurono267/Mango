//
// Created by kurono267 on 08.03.18.
//

#include <iostream>
#include "InstanceVK.hpp"
#include "PipelineVK.hpp"
#include "BufferVK.hpp"
#include <mango.hpp>

using namespace mango::vulkan;
using namespace mango;

InstanceVK::InstanceVK(){

}

InstanceVK::~InstanceVK(){

}

void InstanceVK::release(){
	std::cout << "~InstanceVK" << std::endl;
	_device->release();
	_device.reset();
}

mango::spDevice InstanceVK::device(){
	return _device;
}

void InstanceVK::init(const std::string& title, void* view,const glm::ivec2& size) {
    _size = size;
    _title = title;

    createInstance();
    setupDebugCallback();
    createSurface(view);

    _device = std::make_shared<DeviceVK>();
    dynamic_cast<DeviceVK*>(_device.get())->create(_instance,_surface,size);
}

#ifdef TARGET_OS_IOS

void InstanceVK::createSurface(void* view){
    VkIOSSurfaceCreateInfoMVK surfaceCreateInfo = {};
    surfaceCreateInfo.sType = VK_STRUCTURE_TYPE_IOS_SURFACE_CREATE_INFO_MVK;
    surfaceCreateInfo.pNext = NULL;
    surfaceCreateInfo.flags = 0;
    surfaceCreateInfo.pView = view;
    VkSurfaceKHR surface;
    auto err = vkCreateIOSSurfaceMVK(_instance, &surfaceCreateInfo, nullptr, &surface);
    if(err != VK_SUCCESS){
        std::cout << "vkCreateIOSSurfaceMVK failed" << std::endl;
    }
    _surface = surface;
}
#else

void InstanceVK::createSurface(void* window){
    VkSurfaceKHR surface;
    if (glfwCreateWindowSurface(_instance, window, nullptr, &surface) != VK_SUCCESS){
        throw std::runtime_error("failed to create window surface!");
    }
    _surface = surface;
}
#endif

VkResult vkCreateDebugReportCallbackEXT(VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback) {
    auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugReportCallbackEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pCallback);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void vkDestroyDebugReportCallbackEXT(VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator) {
    auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugReportCallbackEXT");
    if (func != nullptr) {
        func(instance, callback, pAllocator);
    }
}

// Create instance
void InstanceVK::createInstance() {
    vk::ApplicationInfo appInfo(_title.c_str(),VK_MAKE_VERSION(1, 0, 0),"Mango",VK_MAKE_VERSION(1, 0, 0),VK_API_VERSION_1_0);

    vk::InstanceCreateInfo createInfo;
    createInfo.setPApplicationInfo(&appInfo);

    auto extensions = getRequiredExtensions();

    createInfo.setEnabledExtensionCount(extensions.size());
    createInfo.setPpEnabledExtensionNames(extensions.data());

	if (enableValidationLayers) {
		createInfo.setEnabledLayerCount(validationLayers.size());
		createInfo.setPpEnabledLayerNames(validationLayers.data());
	} else {
		createInfo.setEnabledLayerCount(0);
	}

    _instance = vk::createInstance(createInfo);
}

// Validation layers
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

bool InstanceVK::checkValidationLayerSupport() {
	auto availableLayers = vk::enumerateInstanceLayerProperties();

	for (const auto& layer : validationLayers) {
		bool layerFound = false;

		for (const auto& layerProperties : availableLayers) {
			if (strcmp(layer, layerProperties.layerName) == 0) {
				layerFound = true;
				break;
			}
		}

		if (!layerFound) {
			return false;
		}
	}

	return true;
}

void InstanceVK::setupDebugCallback() {
	if (!enableValidationLayers) return;

	vk::DebugReportCallbackCreateInfoEXT createInfo;
	createInfo.setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning);
	createInfo.setPfnCallback(debugCallback);

	_callback = _instance.createDebugReportCallbackEXT(createInfo,nullptr);
}

std::vector<const char*> InstanceVK::getRequiredExtensions() {
    std::vector<const char*> extensions;
    #ifdef TARGET_OS_IOS
        extensions.push_back(VK_MVK_IOS_SURFACE_EXTENSION_NAME);
    #else
        unsigned int glfwExtensionCount = 0;
        const char** glfwExtensions;

        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

        for (unsigned int i = 0; i < glfwExtensionCount; i++) {
            extensions.push_back(glfwExtensions[i]);
        }
    #endif

	extensions.push_back(VK_EXT_SWAPCHAIN_COLOR_SPACE_EXTENSION_NAME);

	if (enableValidationLayers) {
		extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
	}

	return extensions;
}
