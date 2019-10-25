//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"
#include <api/Instance.hpp>
#include "DeviceVK.hpp"

namespace mango::vulkan {

class InstanceVK : public InstanceAPI {
	public:
		InstanceVK();
		~InstanceVK() final;

		void init(const std::string& title, GLFWwindow* window,const glm::ivec2& size) final;
		spDevice device() final;

		void release() final;
	protected:
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
