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

		virtual spSemaphore run(const spSemaphore& waitForIt,const int sizeX, const int sizeY = 1, const int sizeZ = 1) final;
	protected:
		virtual void init() final;
		void initCommandBuffer();
	protected:
		vk::PipelineLayout _pipelineLayout;
		vk::Pipeline _pipeline;
		vk::CommandBuffer _commandBuffer;
		glm::ivec3 _size;
		spSemaphoreVK _semaphore;
};

}

#endif //MANGO_COMPUTEVK_HPP
