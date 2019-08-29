#include <iostream>
#include "Swapchain.hpp"
#include "TextureVK.hpp"
#include "DeviceVK.hpp"
#include "ConvertVK.hpp"
#include <api/Utils.hpp>

using namespace mango::vulkan;

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		return {vk::Format::eB8G8R8A8Unorm, vk::ColorSpaceKHR::eSrgbNonlinear};
	}

	std::cout << "Available swap surface format" << std::endl;
	for (const auto& availableFormat : availableFormats) {
		std::cout << mango::to_string(formatVK2Mango(availableFormat.format)) << std::endl;
	}
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eR16G16B16A16Sfloat/* && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear*/) { // TODO Crossplatform support wide channels swapchain
		//	std::cout << vk::to_string(availableFormat.colorSpace) << std::endl;
		//if (availableFormat.format == vk::Format::eB8G8R8A8Srgb && availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear) {
			return availableFormat;
		}
	}

	return availableFormats[0];
}

vk::PresentModeKHR chooseSwapPresentMode(const std::vector<vk::PresentModeKHR> availablePresentModes) {
	for (const auto& availablePresentMode : availablePresentModes) {
		if (availablePresentMode == vk::PresentModeKHR::eMailbox) {
			return availablePresentMode;
		}
	}

	return vk::PresentModeKHR::eFifo;
}

vk::Extent2D chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& capabilities,const glm::ivec2& size) {
	if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
		return capabilities.currentExtent;
	} else {
		vk::Extent2D actualExtent(size.x, size.y);

		actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width));
		actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height));
		
		return actualExtent;
	}
}

void Swapchain::create(const vk::SurfaceKHR& surface,const glm::ivec2& size,
						const uint32_t queueFamilyIndices[2]){
	auto device = Instance::device<DeviceVK>();

	auto vkDevice = device->getDevice();
	SwapchainSupportDetails swapChainSupport = swapchainSupport(device->getPhysicalDevice(),surface);

	vk::SurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
	vk::PresentModeKHR presentMode = chooseSwapPresentMode(swapChainSupport.presentModes);
	vk::Extent2D extent = chooseSwapExtent(swapChainSupport.capabilities,size);

	uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
	if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
		imageCount = swapChainSupport.capabilities.maxImageCount;
	}

	vk::SwapchainCreateInfoKHR createInfo;
	createInfo.setSurface(surface);

	createInfo.setMinImageCount(imageCount);
	createInfo.setImageFormat(surfaceFormat.format);
	createInfo.setImageColorSpace(vk::ColorSpaceKHR::eSrgbNonlinear);
	createInfo.setImageExtent(extent);
	createInfo.setImageArrayLayers(1);
	createInfo.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment);

	//uint32_t queueFamilyIndices[] = {(uint32_t) indices.graphicsFamily, (uint32_t) indices.presentFamily};

	if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
		createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
		createInfo.setQueueFamilyIndexCount(2);
		createInfo.setPQueueFamilyIndices(queueFamilyIndices);
	} else {
		createInfo.setImageSharingMode(vk::SharingMode::eExclusive);
	}

	createInfo.setPreTransform(swapChainSupport.capabilities.currentTransform);
	createInfo.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque);
	createInfo.setPresentMode(presentMode);
	createInfo.setClipped(VK_TRUE);

	//createInfo.setOldSwapchain(SwapchainKHR());

	_swapchain = vkDevice.createSwapchainKHR(createInfo);
	auto imageVK = vkDevice.getSwapchainImagesKHR(_swapchain);
	for(auto i : imageVK){
		auto tex = std::make_shared<TextureVK>();
		tex->create(extent.width,extent.height,1,formatVK2Mango(surfaceFormat.format),TextureType::Output,i);
		tex->transition(vk::ImageLayout::ePresentSrcKHR);
		_imageViews.push_back(tex->createTextureView());
	}
	_imageFormat = surfaceFormat.format;
	_extent = extent;

	//createImageViews(device);
}

void Swapchain::createImageViews(const vk::Device& device){
	_imageViews.resize(_images.size());
	std::cout << "images size " << _images.size() << std::endl;

	for (uint i = 0; i < _images.size(); i++) {
		spTextureVK tex = std::dynamic_pointer_cast<TextureVK>(_images[i]);
		_imageViews[i] = tex->createTextureView();
	}
}

void Swapchain::release(){
	std::cout << "Swapchain release" << std::endl;
	auto device = Instance::device<DeviceVK>();
	for (uint i = 0; i < _imageViews.size(); ++i) {
		auto view = std::dynamic_pointer_cast<TextureViewVK>(_imageViews[i]);
		device->getDevice().destroyImageView(view->getView());
	}
	device->getDevice().destroySwapchainKHR(_swapchain);
}

Swapchain::~Swapchain() {
	std::cout << "~Swapchain" << std::endl;
}

vk::SwapchainKHR Swapchain::getSwapchain() const {return _swapchain;}
vk::Format    Swapchain::getFormat() const {return _imageFormat;}
vk::Extent2D  Swapchain::getExtent() const {return _extent;}
std::vector<mango::spTextureView> Swapchain::getImageViews() const {return _imageViews;}

SwapchainSupportDetails Swapchain::swapchainSupport(vk::PhysicalDevice device,vk::SurfaceKHR surface){
	SwapchainSupportDetails details;

	details.capabilities = device.getSurfaceCapabilitiesKHR(surface);
	details.formats      = device.getSurfaceFormatsKHR(surface);
	details.presentModes = device.getSurfacePresentModesKHR(surface);

	return details;
}
