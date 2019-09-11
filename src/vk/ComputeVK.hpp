//
// Created by kurono267 on 2019-09-05.
//

#ifndef MANGO_COMPUTEVK_HPP
#define MANGO_COMPUTEVK_HPP

#include <api/Compute.hpp>
#include "default.hpp"
#include "DeviceVK.hpp"

namespace mango::vulkan {

class ComputeVK : public mango::Compute {
	public:
		ComputeVK(const std::string& filename, const std::vector<spDescSet>& descSets);

		vk::Pipeline getPipeline();
		vk::PipelineLayout getLayout();
	protected:
		virtual void init() final;
	protected:
		vk::PipelineLayout _pipelineLayout;
		vk::Pipeline _pipeline;
};

}

#endif //MANGO_COMPUTEVK_HPP
