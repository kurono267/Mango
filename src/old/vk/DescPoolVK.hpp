//
// Created by kurono267 on 2020-08-11.
//

#ifndef MANGOWORLD_DESCPOOLVK_HPP
#define MANGOWORLD_DESCPOOLVK_HPP

#include "default.hpp"
#include "../api/DescPool.hpp"
#include "DescLayoutVK.hpp"

namespace mango::vulkan {

class DescPoolVK final : public DescPool, public std::enable_shared_from_this<DescPoolVK> {
    public:
        DescPoolVK(const mango::spDescLayout &layout, size_t poolSize = DescPoolMin);
        ~DescPoolVK() final;

        virtual spDescSet get(size_t id) final;

        spDescLayout getLayout() final;
	protected:
		struct DescSetStore {
			spDescSet set;
			vk::DescriptorPool pool;
		};

		std::vector<vk::DescriptorPool> _pool;
		std::vector<DescSetStore> _sets;
		spDescLayoutVK _layout;
		size_t _poolSize;
	private:
		vk::DescriptorPool createPool(const spDescLayout& layout, size_t count);
		void appendDescSets(const vk::DescriptorPool& pool, size_t count);
		void increasePool(size_t count);
};

}

#endif //MANGOWORLD_DESCPOOLVK_HPP
