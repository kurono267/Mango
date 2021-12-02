//
// Created by kurono267 on 02.12.2021.
//

#pragma once

#include "../GPUInit.hpp"

class MetalInit : public mango::GPUInit {
	public:
		void init() final;

		void* device;
};



