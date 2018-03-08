//
// Created by kurono267 on 08.03.18.
//

#pragma once

#include "default.hpp"

namespace mango {

class Device {
	public:
		Device() = default;
		virtual ~Device() = default;

		virtual std::string device_name() = 0;
};

typedef std::shared_ptr<Device> spDevice;

};

