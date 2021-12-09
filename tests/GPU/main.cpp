//
// Created by kurono267 on 02.12.2021.
//

#include "MetalTest.hpp"
#include "VulkanTest.hpp"
#include "Resources.hpp"

TEST_F(VulkanTest, InitVulkan) {
	ASSERT_TRUE(status);
}

TEST_F(MetalTest, InitMetal) {
	ASSERT_TRUE(status);
}

TEST_F(VulkanTest, CreateTextureVulkan) {
	textureCreateTest();
}

TEST_F(MetalTest, CreateTextureMetal) {
	textureCreateTest();
}

TEST_F(VulkanTest, CreateBufferVulkan) {
	bufferCreateTest();
}

TEST_F(MetalTest, CreateBufferMetal) {
	bufferCreateTest();
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}