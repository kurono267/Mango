//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "Device.hpp"
#include "Buffer.hpp"

namespace mango {

class Instance {
	public:
		Instance(){}
		virtual ~Instance() = default;
		Instance(const Instance& i) = delete;

		virtual void init(const std::string& title, GLFWwindow* window,const glm::ivec2& size) = 0;
		virtual spDevice device() = 0;
};

};
