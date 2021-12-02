//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "../api/default.hpp"
#include "../api/Instance.hpp"
#include "../api/Device.hpp"

namespace mango::vulkan {

class InstanceVKImpl;

class InstanceVK : public InstanceAPI {
	public:
		InstanceVK();
		~InstanceVK() final;

		void init(const std::string& title, void* window,const glm::ivec2& size) final;
		spDevice device() final;

		void release() final;
	protected:
		InstanceVKImpl* _impl;

		spDevice _device;

		glm::ivec2 _size;
		std::string _title;
};

};
