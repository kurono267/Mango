//
// Created by kurono267 on 02.12.2021.
//

#include "MetalInit.hpp"
#include <stdexcept>
#include <Metal/Metal.h>

void MetalInit::init() {
	device = (void*)MTLCreateSystemDefaultDevice();
	if(!device){
		throw std::runtime_error("Metal failed to create device");
	}
}
