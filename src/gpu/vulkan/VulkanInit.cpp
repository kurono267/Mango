//
// Created by kurono267 on 18.06.2021.
//

#include "VulkanInit.hpp"
#include <iostream>
#ifdef MANGO_GLFW
#include <GLFW/glfw3.h>
#endif

const std::vector<const char*> validationLayers = {
		"VK_LAYER_KHRONOS_validation"
};

const std::vector<const char*> deviceExtensions = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_MAINTENANCE1_EXTENSION_NAME,
		VK_KHR_MAINTENANCE2_EXTENSION_NAME,
		VK_KHR_MAINTENANCE3_EXTENSION_NAME,
		"VK_KHR_portability_subset"
};

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

// Validation layers
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData) {
	std::cerr << "validation layer: " << msg << std::endl;

	return VK_FALSE;
}

bool QueueFamilyIndices::isComplete() {
	return graphicsFamily >= 0 && transferFamily >= 0 && computeFamily >= 0;
}

void QueueFamilyIndices::add(int familyID, const vk::QueueFamilyProperties& family){
	if(family.queueCount <= 0) return;
	if(family.queueFlags & vk::QueueFlagBits::eGraphics){
		graphics.insert(familyID);
	}
	if(family.queueFlags & vk::QueueFlagBits::eCompute){
		compute.insert(familyID);
	}
	if(family.queueFlags & vk::QueueFlagBits::eTransfer){
		transfer.insert(familyID);
	}
}

void QueueFamilyIndices::select() {
	// Select graphics
	if (graphics.empty() || transfer.empty() || compute.empty()) {
		throw std::runtime_error("Failed to select queue family");
	}
	std::set<int> families[3] = {graphics,transfer,compute};
	int selected[3] = {-1,-1,-1};
	for(int i = 0;i<3;++i){
		selected[i] = *families[i].begin();
		for(int j = i+1;j<3;++j){
			if(families[j].size() == 1)continue;
			families[j].erase(selected[i]);
		}
	}
	graphicsFamily = selected[0];
	transferFamily = selected[1];
	computeFamily = selected[2];
}

void VulkanInit::init() {
	std::cout << "VulkanInit" << std::endl;
	vk::ApplicationInfo appInfo("MangoApp",VK_MAKE_VERSION(1, 0, 0),"Mango",VK_MAKE_VERSION(1, 0, 0),VK_API_VERSION_1_2);

	vk::InstanceCreateInfo createInfo;
	createInfo.setPApplicationInfo(&appInfo);

	std::vector<const char*> extensions;
#ifdef DEBUG
	extensions.push_back(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
#endif
#ifdef MANGO_GLFW
	unsigned int glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	for (unsigned int i = 0; i < glfwExtensionCount; i++) {
		extensions.push_back(glfwExtensions[i]);
	}
#endif
	extensions.push_back(VK_KHR_SURFACE_EXTENSION_NAME);

	createInfo.setEnabledExtensionCount(extensions.size());
	createInfo.setPpEnabledExtensionNames(extensions.data());

#ifdef DEBUG
	createInfo.setEnabledLayerCount(validationLayers.size());
	createInfo.setPpEnabledLayerNames(validationLayers.data());
#else
	createInfo.setEnabledLayerCount(0);
#endif
	instance = vk::createInstance(createInfo);
	if(!instance){
		throw std::runtime_error("VulkanInit: Create instance failed");
	}
#ifdef DEBUG
	vk::DebugReportCallbackCreateInfoEXT debugCreateInfo;
	debugCreateInfo.setFlags(vk::DebugReportFlagBitsEXT::eError | vk::DebugReportFlagBitsEXT::eWarning);
	debugCreateInfo.setPfnCallback(debugCallback);

	instance.createDebugReportCallbackEXT(debugCreateInfo,nullptr);
#endif

	auto physicalDevices = instance.enumeratePhysicalDevices();
	if(physicalDevices.empty()){
		throw std::runtime_error("VulkanInit: Failed to find GPUs with Vulkan support!");
	}

	pDevice = physicalDevices[0];
	// Find discrete GPU
	for(auto device : physicalDevices){
		auto property = device.getProperties();
		if(property.deviceType == vk::PhysicalDeviceType::eDiscreteGpu){
			pDevice = device;
			break;
		}
	}

	// Get queue families
	auto queueFamilies = pDevice.getQueueFamilyProperties();
	for(int i = 0;i<queueFamilies.size();++i){
		familyIndices.add(i,queueFamilies[i]);
	}
	familyIndices.select();

	// Create logical device
	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = {familyIndices.graphicsFamily, familyIndices.transferFamily, familyIndices.computeFamily};

	float queuePriority = 1.0f;
	for(int queueFamily : uniqueQueueFamilies){
		vk::DeviceQueueCreateInfo queueCreateInfo;
		queueCreateInfo.setQueueFamilyIndex(queueFamily);
		queueCreateInfo.setQueueCount(1);
		queueCreateInfo.setPQueuePriorities(&queuePriority);
		queueCreateInfos.push_back(queueCreateInfo);
	}

	vk::PhysicalDeviceFeatures deviceFeatures;
	deviceFeatures.samplerAnisotropy = true;
	deviceFeatures.tessellationShader = true;
	deviceFeatures.multiViewport = true;
	deviceFeatures.fragmentStoresAndAtomics = true;
	//deviceFeatures.geometryShader = true; // MoltenVK doesn't support geometry shader feature

	vk::DeviceCreateInfo deviceCreateInfo;

	deviceCreateInfo.setPQueueCreateInfos(queueCreateInfos.data());
	deviceCreateInfo.setQueueCreateInfoCount(queueCreateInfos.size());

	deviceCreateInfo.setPEnabledFeatures(&deviceFeatures);

	deviceCreateInfo.setPpEnabledExtensionNames(deviceExtensions.data());
	deviceCreateInfo.setEnabledExtensionCount(deviceExtensions.size());

	vk::PhysicalDeviceDescriptorIndexingFeatures indexingFeatures;
	indexingFeatures.runtimeDescriptorArray = true;

	deviceCreateInfo.pNext = &indexingFeatures;

#ifdef DEBUG
	deviceCreateInfo.setEnabledLayerCount(validationLayers.size());
	deviceCreateInfo.setPpEnabledLayerNames(validationLayers.data());
#else
	deviceCreateInfo.setEnabledLayerCount(0);
#endif

	device = pDevice.createDevice(deviceCreateInfo);
	if(!device){
		throw std::runtime_error("VulkanInit: Create logical device failed");
	}
}
