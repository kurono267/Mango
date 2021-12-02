//
// Created by kurono267 on 28.07.2021.
//

#pragma once

#include <gtest/gtest.h>
#include <mango.hpp>

class GPUTest : public ::testing::Test {
	public:
		GPUTest();
		~GPUTest();
		void SetUp() override;
		void TearDown() override;
	public:
		bool status;
};


