//
// Created by kurono267 on 2020-08-11.
//

#include "DescLayoutVK.hpp"
#include "../api/Instance.hpp"
#include "DeviceVK.hpp"
#include "ConvertVK.hpp"

using namespace mango;

namespace mango::vulkan {

void DescLayoutVK::create() {
    auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::vector<vk::DescriptorSetLayoutBinding> layoutBinds;
    for(auto entry : _entries){
        layoutBinds.emplace_back(entry.binding,descTypeFromMango(entry.type),1,shaderStageVK(entry.stage));
    }

    vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(),layoutBinds.size(),layoutBinds.data());
    _layout = vk_device.createDescriptorSetLayout(layoutInfo);
}

vk::DescriptorSetLayout DescLayoutVK::getLayout() {
    return _layout;
}

vk::DescriptorType descTypeFromMango(const DescType &type) {
	switch(type){
		case DescType::Uniform:
			return vk::DescriptorType::eUniformBuffer;
		case DescType::Storage:
            return vk::DescriptorType::eStorageBuffer;
	    case DescType::StorageTexture:
	        return vk::DescriptorType::eStorageImage;
	    case DescType::Texture:
	        return vk::DescriptorType::eCombinedImageSampler;
	}
}

}
