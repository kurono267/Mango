//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include <api/default.hpp>
#include <api/instance.hpp>
#include "device_vk.hpp"

namespace mango::vulkan {

class InstanceVK : public Instance {
	public:
		InstanceVK();
		~InstanceVK() final;

		void init(const std::string& title, GLFWwindow* window,const glm::ivec2& size) final;
		spDevice device() final;
	protected:
		void initVulkan();
		void createInstance();

		// Validation layers
		std::vector<const char*> getRequiredExtensions();
		void setupDebugCallback();
		bool checkValidationLayerSupport();

		// Create surface
		void createSurface(GLFWwindow* window);

		vk::Instance _instance;
		vk::DebugReportCallbackEXT _callback;
		vk::SurfaceKHR _surface;

		spDevice _device;

		glm::ivec2 _size;
		std::string _title;
};

};
