//
// Created by kurono267 on 18.06.2021.
//

#pragma once

#include <vulkan/vulkan.hpp>
#include <set>
#include "../GPUInit.hpp"

class QueueFamilyIndices {
	public:
		int graphicsFamily = -1;
		int transferFamily = -1;
		int computeFamily = -1;

		bool isComplete();

		void add(int familyID, const vk::QueueFamilyProperties& family);
		void select();
	private:
		std::set<int> graphics;
		std::set<int> transfer;
		std::set<int> compute;
};

struct VulkanData {
	vk::Instance instance;
	vk::PhysicalDevice pDevice;
	vk::Device device;
	QueueFamilyIndices familyIndices;

	bool check();
};

class VulkanInit : public mango::GPUInit {
	public:
		void init() final;

		vk::Instance instance;
		vk::PhysicalDevice pDevice;
		vk::Device device;
		QueueFamilyIndices familyIndices;
};
