#pragma once

#include "default.hpp"
#include "texture_vk.hpp"

namespace mango::vulkan {

struct SwapchainSupportDetails {
	vk::SurfaceCapabilitiesKHR capabilities;
	std::vector<vk::SurfaceFormatKHR> formats;
	std::vector<vk::PresentModeKHR> presentModes;
};

class Swapchain {
	public:
		Swapchain() = default;
		~Swapchain() = default;

		void create(const vk::PhysicalDevice& pDevice,const vk::Device& device,const vk::SurfaceKHR& surface,const glm::ivec2& size,const uint32_t queueFamilyIndices[2]);
		void release();

		vk::SwapchainKHR getSwapchain() const ;
		vk::Format    getFormat() const ;
		vk::Extent2D  getExtent() const ;
		std::vector<mango::spTextureView> getImageViews() const ;

		static SwapchainSupportDetails swapchainSupport(vk::PhysicalDevice device,vk::SurfaceKHR surface);
	protected:
		vk::SwapchainKHR      _swapchain;
		std::vector<mango::spTexture> _images;
		vk::Format         _imageFormat;
		vk::Extent2D       _extent; 

		void createImageViews(const vk::Device& device);
		std::vector<mango::spTextureView> _imageViews;

		vk::Device _device;

		bool _isInit;
};

typedef std::shared_ptr<Swapchain> spSwapchain;

};

