//
// Created by kurono267 on 2020-08-11.
//

#ifndef MANGOWORLD_DESCLAYOUTVK_HPP
#define MANGOWORLD_DESCLAYOUTVK_HPP

#include "../api/DescLayout.hpp"
#include "default.hpp"

namespace mango::vulkan {

class DescLayoutVK : public DescLayout {
	public:
		void create() final;

		vk::DescriptorSetLayout getLayout();
	private:
		vk::DescriptorSetLayout _layout;
};

vk::DescriptorType descTypeFromMango(const DescType& type);

typedef std::shared_ptr<DescLayoutVK> spDescLayoutVK;

}

#endif //MANGOWORLD_DESCLAYOUTVK_HPP
