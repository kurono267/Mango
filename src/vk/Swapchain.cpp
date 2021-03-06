#include <iostream>
#include "Swapchain.hpp"
#include "TextureVK.hpp"
#include "DeviceVK.hpp"
#include "ConvertVK.hpp"
#include "../api/Utils.hpp"
#include "../api/Instance.hpp"
#include <sstream>

using namespace mango::vulkan;

std::string swapSurfaceFormatString(const vk::SurfaceFormatKHR& format){
	std::stringstream stream;
	stream << "Format: " << mango::to_string(formatVK2Mango(format.format)) << " ColorSpace: ";
	stream << vk::to_string(format.colorSpace);
	return stream.str();
}

vk::SurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<vk::SurfaceFormatKHR>& availableFormats) {
	if (availableFormats.size() == 1 && availableFormats[0].format == vk::Format::eUndefined) {
		VkSurfaceFormatKHR formatSurface;
		formatSurface.format = (VkFormat)vk::Format::eB8G8R8A8Unorm;
		formatSurface.colorSpace = (VkColorSpaceKHR)vk::ColorSpaceKHR::eSrgbNonlinear;
		return formatSurface;
	}

	std::cout << "Available swap surface format" << std::endl;
	for (const auto& availableFormat : availableFormats) {
		std::cout << swapSurfaceFormatString(availableFormat) << std::endl;
	}
	std::cout << "Selected swap surface " << std::endl;
	for (const auto& availableFormat : availableFormats) {
		if (availableFormat.format == vk::Format::eR16G16B16A16Sfloat && availableFormat.colorSpace == vk::ColorSpaceKHR::eDciP3LinearEXT) { // TODO Improve selection format of swapchain
			std::cout << swapSurfaceFormatString(availableFormat) << std::endl;
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
	createInfo.setImageColorSpace(surfaceFormat.colorSpace);
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
		tex->create(extent.width,extent.height,1,formatVK2Mango(surfaceFormat.format),TextureUsage::ColorAttachment | TextureUsage::Sampled,i);
		tex->transition(TextureLayout::PresentSrcKHR);
		_images.push_back(tex);
		_imageViews.push_back(tex->createTextureView());
	}
	_imageFormat = surfaceFormat.format;
	_extent = extent;
}

Swapchain::~Swapchain() {
	std::cout << "~Swapchain" << std::endl;
	spDeviceVK deviceVK = Instance::device<DeviceVK>();
	deviceVK->getDevice().destroySwapchainKHR(_swapchain);
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
