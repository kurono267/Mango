//
// Created by kurono267 on 28.07.2021.
//

#include "VulkanTest.hpp"

using namespace mango;

VulkanTest::VulkanTest() {
	try {
		GPU::init(mango::Vulkan);
		status = true;
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		status = false;
	}
}

VulkanTest::~VulkanTest() {

}

void VulkanTest::SetUp() {
	Test::SetUp();
}

void VulkanTest::TearDown() {
	Test::TearDown();
}
