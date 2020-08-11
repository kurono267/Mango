//
// Created by kurono267 on 2020-08-11.
//

#include "DescPoolVK.hpp"
#include "../api/Instance.hpp"
#include "DeviceVK.hpp"
#include "DescSetVK.hpp"

using namespace mango;

namespace mango::vulkan {

DescPoolVK::DescPoolVK(size_t numDescSets,const mango::spDescLayout &layout) : DescPool(),_layout(std::dynamic_pointer_cast<DescLayoutVK>(layout)) {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::vector<vk::DescriptorPoolSize>         poolSizes;
	for(const auto& entry : layout->getEntries()){
		poolSizes.emplace_back(descTypeFromMango(entry.type),1);
	}

	vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags() | vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,numDescSets,poolSizes.size(),poolSizes.data());
	_pool = vk_device.createDescriptorPool(poolInfo);
}

std::vector<spDescSet> DescPoolVK::create(size_t count) {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	vk::DescriptorSetLayout layout = _layout->getLayout();
	vk::DescriptorSetAllocateInfo allocInfo(_pool,count,&layout);

	std::vector<spDescSet> results(count);
	auto descSetList = vk_device.allocateDescriptorSets(allocInfo);
	for(int i = 0;i<count;++i){
		vk::DescriptorSet descSet = descSetList[i];
		results[i] = std::make_shared<DescSetVK>(descSet,shared_from_this());
	}

	return results;
}

spDescSet DescPoolVK::create() {
	return create(1)[0];
}

spDescLayout DescPoolVK::getLayout() {
	return _layout;
}

}
