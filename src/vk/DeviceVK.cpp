//
// Created by kurono267 on 08.03.18.
//

#include <iostream>
#include <set>
#include "../api/Texture.hpp"
#include "DeviceVK.hpp"
#include "PipelineVK.hpp"
#include "BufferVK.hpp"
#include "TextureVK.hpp"
#include "FramebufferVK.hpp"
#include "DescSetVK.hpp"
#include "ComputeVK.hpp"
#include "../unified/RenderTarget.hpp"

using namespace mango::vulkan;
using namespace mango;

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
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

DeviceVK::~DeviceVK(){
	std::cout << "~DeviceVK" << std::endl;
	_device.destroyCommandPool(_pool);
	_device.destroyCommandPool(_computePool);
	_device.destroy();
}

void DeviceVK::release() {
	std::cout << "DeviceVK::release" << std::endl;
	_screenRenderTargets.clear();

	_swapchain.reset();
}

void DeviceVK::create(const vk::Instance& instance,const vk::SurfaceKHR& surface,const glm::ivec2& size){
	_instance = instance;_surface = surface;_size = size;
	pickPhysicalDevice();
	createLogicalDevice();

	auto queueFamily = queueFamilies(_pDevice,_surface);
	uint32_t familyIndices[2] = {(uint32_t)queueFamily.graphicsFamily,(uint32_t)queueFamily.presentFamily};

	vk::CommandPoolCreateInfo poolInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer,queueFamily.graphicsFamily);
	_pool = _device.createCommandPool(poolInfo);
	vk::CommandPoolCreateInfo poolComputeInfo(vk::CommandPoolCreateFlagBits::eResetCommandBuffer,queueFamily.computeFamily);
	_computePool = _device.createCommandPool(poolComputeInfo);

	_swapchain = std::make_unique<Swapchain>();
	_swapchain->create(_surface,size,familyIndices);

	createScreen();
}

void DeviceVK::pickPhysicalDevice(){
	auto devices = _instance.enumeratePhysicalDevices();

	if (devices.empty()) {
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
		requiredExtensions.erase(std::string(extension.extensionName));
	}

	if(!requiredExtensions.empty()){
		std::cout << "This extensions doesn't support:" << std::endl;
		for(const auto& ext : requiredExtensions){
			std::cout << ext << std::endl;
		}
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
	//deviceFeatures.geometryShader = true; // MoltenVK doesn't support geometry shader feature

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

std::string DeviceVK::deviceName(){
	vk::PhysicalDeviceProperties pdProp = _pDevice.getProperties();
	return std::string(pdProp.deviceName);
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

mango::spPipeline DeviceVK::createPipeline(const PipelineInfo& rp){
	return PipelineVK::make(rp);
}

mango::spBuffer DeviceVK::createBuffer(const BufferType& type,const MemoryType& memory,const size_t& size,void* data){
	auto buffer = std::make_shared<BufferVK>();
	buffer->create(type,memory,size,data);
	return buffer;
}

mango::spRenderPass DeviceVK::createRenderPass(){
	return std::make_shared<RenderPassVK>();
}

mango::spTexture DeviceVK::createTexture(int width, int height, int miplevels, const mango::Format &format,
                                  const TextureType &type) {
	auto texture = std::make_shared<TextureVK>();
	texture->create(width,height,miplevels,format,type);
	return texture;
}

mango::spTexture DeviceVK::createTexture3D(int width, int height, int depth, int miplevels, const Format& format, const TextureType& type) {
	auto texture = std::make_shared<TextureVK>();
	texture->create3D(width,height,depth,miplevels,format,type);
	return texture;
}

spTexture DeviceVK::createTexture() {
	return std::make_shared<TextureVK>();
}

spFramebuffer DeviceVK::createFramebuffer(const glm::ivec2& size) {
	return std::make_shared<FramebufferVK>(size);
}

spFramebuffer DeviceVK::createFramebuffer(const int width, const int height) {
	return std::make_shared<FramebufferVK>(width,height);
}

mango::spCommandBuffer DeviceVK::createCommandBuffer(){
	auto cmd = std::make_shared<CommandBufferVK>();
	cmd->create();
	return cmd;
}

vk::Format supportedFormat(vk::PhysicalDevice _pDevice,const std::vector<vk::Format>& candidates, vk::ImageTiling tiling,const vk::FormatFeatureFlags& features){
	for (vk::Format format : candidates) {
		vk::FormatProperties props = _pDevice.getFormatProperties(format);

		if (tiling == vk::ImageTiling::eLinear && (props.linearTilingFeatures & features) == features) {
			return format;
		} else if (tiling == vk::ImageTiling::eOptimal && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}

	throw std::runtime_error("failed to find supported format!");
}

mango::Format DeviceVK::getDepthFormat() {
	vk::Format format = supportedFormat(_pDevice,
        {vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint, vk::Format::eD24UnormS8Uint},
        vk::ImageTiling::eOptimal,
        vk::FormatFeatureFlagBits::eDepthStencilAttachment
    );
    return formatVK2Mango(format);
}

void DeviceVK::createScreen(){
	auto screenRenderPass = std::make_shared<RenderPassVK>();
	screenRenderPass->addAttachment(Attachment(formatVK2Mango(_swapchain->getFormat()),false,0));
	screenRenderPass->addAttachment(Attachment(getDepthFormat(),true,1, AttachmentLoad::Clear, AttachmentStore::DontCare));
	screenRenderPass->create(true);

	auto imageViews = _swapchain->getImageViews();
	auto extent = _swapchain->getExtent();

	for(const auto& view : imageViews){
		spFramebufferVK framebuffer = std::dynamic_pointer_cast<FramebufferVK>(createFramebuffer(extent.width,extent.height));

		framebuffer->attachment(view);
		framebuffer->depth();

		auto depthTexture = std::dynamic_pointer_cast<TextureVK>(framebuffer->getDepthTexture());
		depthTexture->transition(vk::ImageLayout::eDepthStencilAttachmentOptimal);

		framebuffer->finish(screenRenderPass);

		_screenRenderTargets.push_back(std::make_shared<RenderTarget>(framebuffer,screenRenderPass));
	}
}

std::vector<spRenderTarget> DeviceVK::getScreenRenderTargets(){
	return _screenRenderTargets;
}

mango::spSemaphore DeviceVK::createSemaphore(){
	auto semaphore = std::make_shared<SemaphoreVK>();
	return semaphore;
}

spCompute DeviceVK::createCompute(const std::string& filename, const std::vector<spDescSet>& descSets) {
	return std::make_shared<ComputeVK>(filename,descSets);
}

void DeviceVK::submit(const mango::spCommandBuffer& cmd, const mango::spSemaphore& waitForIt, const mango::spSemaphore& result){
	auto cmd_vk = std::dynamic_pointer_cast<CommandBufferVK>(cmd)->getVK();

	auto waitVK = std::dynamic_pointer_cast<SemaphoreVK>(waitForIt);
	auto signalVK = std::dynamic_pointer_cast<SemaphoreVK>(result);

	vk::Semaphore waitSemaphores[] = {waitVK?waitVK->getVK(): nullptr};
	vk::Semaphore signalSemaphores[] = {signalVK->getVK()};
	vk::PipelineStageFlags waitStages[] = {vk::PipelineStageFlagBits::eColorAttachmentOutput};

	vk::SubmitInfo submitInfo(
		waitVK?1:0, waitVK?waitSemaphores: nullptr,
		waitStages,
		1, &cmd_vk,
		1, signalSemaphores
	);

	_graphicsQueue.submit(submitInfo, nullptr);
}

void DeviceVK::present(uint32_t screen, const mango::spSemaphore& signal){
	vk::SwapchainKHR swapChains[] = {_swapchain->getSwapchain()};

	vk::Semaphore signalSemaphores[] = {std::dynamic_pointer_cast<SemaphoreVK>(signal)->getVK()};

	vk::PresentInfoKHR presentInfo(
		1, signalSemaphores,
		1, swapChains,
		&screen
	);

	_presentQueue.presentKHR(presentInfo);
}

uint32_t DeviceVK::nextScreen(const mango::spSemaphore& signal){
	vk::Semaphore signalSemaphores = std::dynamic_pointer_cast<SemaphoreVK>(signal)->getVK();

	return _device.acquireNextImageKHR(_swapchain->getSwapchain(),std::numeric_limits<uint64_t>::max(),signalSemaphores,nullptr).value;
}

mango::spDescSet DeviceVK::createDescSet() {
    spDescSet descSet = std::make_shared<DescSetVK>();
    return descSet;
}

void DeviceVK::waitIdle() {
	_device.waitIdle();
}

vk::CommandPool DeviceVK::getComputeCommandPool() {
	return _computePool;
}

vk::Queue DeviceVK::getComputeQueue() {
	return _computeQueue;
}

glm::ivec2 DeviceVK::getScreenSize(){
	return _size;
}

SemaphoreVK::SemaphoreVK() {
	_semaphore = Instance::device<DeviceVK>()->getDevice().createSemaphore(vk::SemaphoreCreateInfo());
}

SemaphoreVK::~SemaphoreVK() {
	std::cout << "~SemaphoreVK" << std::endl;
	Instance::device<DeviceVK>()->getDevice().destroySemaphore(_semaphore);
}

vk::Semaphore SemaphoreVK::getVK(){
	return _semaphore;
}
