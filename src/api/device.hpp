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

		virtual spRenderPass createRenderPass() = 0;
		virtual spPipeline createPipeline(const RenderPattern& rp) = 0;
		virtual spBuffer createBuffer(const BufferType& type,const size_t& size,void* data = nullptr) = 0;

		virtual
};

typedef std::shared_ptr<Device> spDevice;

};

