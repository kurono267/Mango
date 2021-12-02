//
// Created by kurono267 on 28.07.2021.
//

#include "GPUTest.hpp"

using namespace mango;

GPUTest::GPUTest() {
	try {
		GPU::init(mango::Vulkan);
		status = true;
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		status = false;
	}
}

GPUTest::~GPUTest() {

}

void GPUTest::SetUp() {
	Test::SetUp();
}

void GPUTest::TearDown() {
	Test::TearDown();
}

TEST_F(GPUTest, Init) {
	ASSERT_TRUE(status);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
