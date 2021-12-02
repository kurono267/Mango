//
// Created by kurono267 on 28.07.2021.
//

#pragma once

#include <gtest/gtest.h>
#include <mango.hpp>

class MetalTest : public ::testing::Test {
	public:
		MetalTest();
		~MetalTest();
		void SetUp() override;
		void TearDown() override;
	public:
		bool status;
};


