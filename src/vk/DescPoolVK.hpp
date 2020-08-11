//
// Created by kurono267 on 2020-08-11.
//

#ifndef MANGOWORLD_DESCPOOLVK_HPP
#define MANGOWORLD_DESCPOOLVK_HPP

#include "default.hpp"
#include "../api/DescPool.hpp"
#include "DescLayoutVK.hpp"

namespace mango::vulkan {

class DescPoolVK : public DescPool, public std::enable_shared_from_this<DescPoolVK> {
    public:
        DescPoolVK(size_t numDescSets,const mango::spDescLayout &layout);

        std::vector<spDescSet> create(size_t count) final;
        spDescSet create() final;

        spDescLayout getLayout() final;
	protected:
		vk::DescriptorPool _pool;
		spDescLayoutVK _layout;
};

}

#endif //MANGOWORLD_DESCPOOLVK_HPP
