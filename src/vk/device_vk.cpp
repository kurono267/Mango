//
// Created by kurono267 on 08.03.18.
//

#include <iostream>
#include <set>
#include <mango.hpp>
#include <api/texture.hpp>
#include "device_vk.hpp"
#include "pipeline_vk.hpp"
#include "buffer_vk.hpp"
#include "texture_vk.hpp"

using namespace mango::vulkan;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_KHR_MULTIVIEW_EXTENSION_NAME
};

struct QueueFamilyIndices {
	int graphicsFamily = -1;
	int presentFamily = -1;
	int computeFamily = -1;

	bool isComplete() {
		return graphicsFamily >= 0 && presentFamily >= 0 && computeFamily >= 0;
	}
};

QueueFamilyIndices queueFamilies(const vk::PhysicalDevice& device,const vk::SurfaceKHR surface) {
	QueueFamilyIndices indices;

	auto queueFamilies = device.getQueueFamilyProperties();

	int i = 0;
	for (const auto& queueFamily : queueFamilies) {
		if (indices.graphicsFamily == -1 && queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eGraphics) {
			indices.graphicsFamily = i;
		}

		vk::Bool32 presentSupport = device.getSurfaceSupportKHR(i,surface);

		if(queueFamily.queueCount > 0 && presentSupport){
			indices.presentFamily = i;
		}

		if(queueFamily.queueCount > 0 && queueFamily.queueFlags & vk::QueueFlagBits::eCompute){
			indices.computeFamily = i;
		}

		/*if (indices.isComplete()) {
			break;
		}*/

		i++;
	}

	return indices;
}

DeviceVK::DeviceVK(){}
DeviceVK::~DeviceVK(){
	_swapchain.release();
	std::cout << "Device Destructor" << std::endl;
}

void DeviceVK::create(const vk::Instance& instance,const vk::SurfaceKHR& surface,const glm::ivec2& size){
	_instance = instance;_surface = surface;_size = size;
	pickPhysicalDevice();
	createLogicalDevice();

	auto queueFamily = queueFamilies(_pDevice,_surface);
	uint32_t familyIndices[2] = {queueFamily.graphicsFamily,queueFamily.presentFamily};

	_swapchain.create(_pDevice,_device,_surface,size,familyIndices);

	vk::CommandPoolCreateInfo poolInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer,queueFamily.graphicsFamily);
	_pool = _device.createCommandPool(poolInfo);
	vk::CommandPoolCreateInfo poolComputeInfo(vk::CommandPoolCreateFlags(),queueFamily.computeFamily);
	//_poolCompute = _device.createCommandPool(poolComputeInfo);
}

void DeviceVK::pickPhysicalDevice(){
	auto devices = _instance.enumeratePhysicalDevices();

	if (devices.size() == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	bool haveDevice = false;
	for (const auto& device : devices) {
		if (isDeviceSuitable(device)) {
			_pDevice = device;
			haveDevice = true;
			break;
		}
	}

	if (!haveDevice) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

bool checkDeviceExtensionSupport(vk::PhysicalDevice device) {
	auto availableExtensions = device.enumerateDeviceExtensionProperties();

	std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

	for (const auto& extension : availableExtensions) {
		requiredExtensions.erase(extension.extensionName);
	}

	return requiredExtensions.empty();
}

bool DeviceVK::isDeviceSuitable(const vk::PhysicalDevice& device){
	QueueFamilyIndices indices = queueFamilies(device,_surface);

	bool extensionsSupported = checkDeviceExtensionSupport(device);
	bool swapChainAdequate = false;
	if(extensionsSupported){
		SwapchainSupportDetails swapChainSupport = Swapchain::swapchainSupport(device,_surface);
		swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
	}

	vk::PhysicalDeviceFeatures supportedFeatures = device.getFeatures();

	return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
}

void DeviceVK::createLogicalDevice(){
	QueueFamilyIndices indices = queueFamilies(_pDevice,_surface);

	std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
	std::set<int> uniqueQueueFamilies = {indices.graphicsFamily, indices.presentFamily};

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
	deviceFeatures.geometryShader = true;

	vk::DeviceCreateInfo createInfo;

	createInfo.setPQueueCreateInfos(queueCreateInfos.data());
	createInfo.setQueueCreateInfoCount(queueCreateInfos.size());

	createInfo.setPEnabledFeatures(&deviceFeatures);

	createInfo.setPpEnabledExtensionNames(deviceExtensions.data());
	createInfo.setEnabledExtensionCount(deviceExtensions.size());

	if (enableValidationLayers) {
		createInfo.setEnabledLayerCount(validationLayers.size());
		createInfo.setPpEnabledLayerNames(validationLayers.data());
	} else {
		createInfo.setEnabledLayerCount(0);
	}

	_device = _pDevice.createDevice(createInfo);
	_presentQueue  = _device.getQueue(indices.presentFamily,0);
	_graphicsQueue  = _device.getQueue(indices.graphicsFamily,0);
	_computeQueue  = _device.getQueue(indices.computeFamily,0);
}

std::string DeviceVK::device_name(){
	vk::PhysicalDeviceProperties pdProp = _pDevice.getProperties();
	return pdProp.deviceName;
}

vk::Device DeviceVK::getDevice() {
	return _device;
}

vk::PhysicalDevice DeviceVK::getPhysicalDevice(){
	return _pDevice;
}

vk::CommandPool DeviceVK::getCommandPool(){
	return _pool;
}

vk::Queue DeviceVK::getGraphicsQueue(){
	return _graphicsQueue;
}

mango::spPipeline DeviceVK::createPipeline(const RenderPattern& rp){
	return PipelineVK::make(rp);
}

mango::spBuffer DeviceVK::createBuffer(const BufferType& type,const size_t& size,void* data){
	auto buffer = std::make_shared<BufferVK>();
	buffer->create(shared_from_this(),type,size,data);
	return buffer;
}

mango::spRenderPass DeviceVK::createRenderPass(){
	return std::make_shared<RenderPassVK>();
}

mango::spTexture DeviceVK::createTexture(const int width, const int height, const int miplevels, const mango::Format &format,
                                  const TextureType &type, const void *data) {
	auto texture = std::make_shared<TextureVK>();
	texture->create(shared_from_this(),width,height,miplevels,format,type,data);
	return texture;
}
