//
// Created by kurono267 on 2020-08-11.
//

#include "DescPoolVK.hpp"
#include "../api/Instance.hpp"
#include "DeviceVK.hpp"
#include "DescSetVK.hpp"

using namespace mango;

namespace mango::vulkan {

DescPoolVK::DescPoolVK(const mango::spDescLayout &layout, size_t poolSize) : DescPool(),_poolSize(poolSize),_layout(std::dynamic_pointer_cast<DescLayoutVK>(layout)) {
	increasePool(_poolSize);
}

DescPoolVK::~DescPoolVK() {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();
	_sets.clear();

	for(int i = 0;i<_sets.size();++i){
		auto store = _sets[i];
		auto set = std::static_pointer_cast<DescSetVK>(store.set);
		auto vkSet = set->getSet();
		vk_device.freeDescriptorSets(store.pool,1,&vkSet);
	}

	for(const auto& pool : _pool){
		vk_device.destroyDescriptorPool(pool);
	}
}

spDescLayout DescPoolVK::getLayout() {
	return _layout;
}

spDescSet DescPoolVK::get(size_t id) {
	if(id >= _sets.size()){
		uint32_t increaseSize = std::min(id-_sets.size()-1,_poolSize);
		increasePool(increaseSize);
	}
	return _sets[id].set;
}

vk::DescriptorPool DescPoolVK::createPool(const spDescLayout &layout, size_t count) {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::vector<vk::DescriptorPoolSize>         poolSizes;
	for(const auto& entry : layout->getEntries()){
		poolSizes.emplace_back(descTypeFromMango(entry.type),count);
	}

	vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags() | vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,count,poolSizes.size(),poolSizes.data());
	auto pool = vk_device.createDescriptorPool(poolInfo);
	return pool;
}

void DescPoolVK::appendDescSets(const vk::DescriptorPool& pool, size_t count) {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	vk::DescriptorSetLayout layout = _layout->getLayout();
	std::vector<vk::DescriptorSetLayout> layouts(count);
	for(int i = 0;i<count;++i){
		layouts[i] = layout;
	}
	vk::DescriptorSetAllocateInfo allocInfo(pool,count,layouts.data());

	std::vector<spDescSet> results(count);
	auto descSetList = vk_device.allocateDescriptorSets(allocInfo);
	for(int i = 0;i<count;++i){
		vk::DescriptorSet descSet = descSetList[i];
		DescSetStore store;
		store.set = std::make_shared<DescSetVK>(descSet,_layout->getLayout());
		store.pool = pool;
		_sets.push_back(store);
	}
}

void DescPoolVK::increasePool(size_t count) {
	_pool.emplace_back(createPool(_layout,count));
	appendDescSets(_pool.back(),count);
}

}
