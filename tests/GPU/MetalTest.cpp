//
// Created by kurono267 on 28.07.2021.
//

#include "MetalTest.hpp"

using namespace mango;

MetalTest::MetalTest() {
	try {
		GPU::init(mango::Metal);
		status = true;
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		status = false;
	}
}

MetalTest::~MetalTest() {

}

void MetalTest::SetUp() {
	Test::SetUp();
}

void MetalTest::TearDown() {
	Test::TearDown();
}
