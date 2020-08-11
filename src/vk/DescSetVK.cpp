//
// Created by kurono267 on 26.11.2018.
//

#include "DescSetVK.hpp"
#include "BufferVK.hpp"
#include "TextureVK.hpp"
#include <set>
#include <iostream>
#include "../api/Instance.hpp"

namespace mango::vulkan {

DescSetVK::DescSetVK() {}

DescSetVK::DescSetVK(const vk::DescriptorSet &descSet, const spDescPool& pool) : _descSet(descSet), _descPool(pool) {

}

DescSetVK::~DescSetVK() {
	std::cout << "~DescSetVK" << std::endl;
	/*auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::set<vk::Sampler> samplerSet;
	for(auto s : _samplerBinds){
		samplerSet.insert(s.sampler);
	}

	for(auto s : samplerSet){
		vk_device.destroySampler(s);
	}

	vk_device.freeDescriptorSets(_descPool,1,&_descSet);
	vk_device.destroyDescriptorPool(_descPool);
	vk_device.destroyDescriptorSetLayout(_descLayout);*/
}
/*
void DescSetVK::create(){
    // Create Decription Set Layout
    std::vector<vk::DescriptorSetLayoutBinding> layoutBinds;
    std::vector<vk::DescriptorPoolSize>         poolSizes;

    auto vk_device = Instance::device<DeviceVK>()->getDevice();

    std::unordered_map<int,int> typesDesc;
    // Add Layout binding for UBO
    for(const auto& u : _uboBinds){
        layoutBinds.push_back(vk::DescriptorSetLayoutBinding(u.binding,u.descType,1,u.stage));
        poolSizes.push_back(vk::DescriptorPoolSize(u.descType,1));
        if(typesDesc.find((int)u.descType) == typesDesc.end()){
            typesDesc.insert(std::pair<int,int>((int)u.descType,1));
        } else {
            typesDesc[(int)u.descType] += 1;
        }
    }
    for(const auto& s : _samplerBinds) {
        layoutBinds.push_back(
                vk::DescriptorSetLayoutBinding(s.binding, s.descType, 1, s.stage));
        poolSizes.push_back(vk::DescriptorPoolSize(s.descType, 1));
        if(typesDesc.find((int)s.descType) == typesDesc.end()){
            typesDesc.insert(std::pair<int,int>((int)s.descType,1));
        } else {
            typesDesc[(int)s.descType] += 1;
        }
    }

    vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(),layoutBinds.size(),layoutBinds.data());
    _descLayout = vk_device.createDescriptorSetLayout(layoutInfo);

    vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags() | vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,1,poolSizes.size(),poolSizes.data());
    _descPool = vk_device.createDescriptorPool(poolInfo);

    vk::DescriptorSetAllocateInfo allocInfo(_descPool,1,&_descLayout);
    _descSet = vk_device.allocateDescriptorSets(allocInfo)[0];

    std::vector<vk::WriteDescriptorSet>         descWrites;
    _descBufferInfos.resize(_uboBinds.size());
    for(auto u : _uboBinds){
        auto internalBuffer = std::dynamic_pointer_cast<BufferVK>(u.buffer.getBuffer());
        _descBufferInfos.push_back(vk::DescriptorBufferInfo(internalBuffer->getVKBuffer(),u.offset,u.size));
        descWrites.push_back(vk::WriteDescriptorSet(_descSet,u.binding,0,1,u.descType,nullptr,&_descBufferInfos[_descBufferInfos.size()-1],nullptr));
    }
    _descImageInfos.resize(_samplerBinds.size());
    for(auto s : _samplerBinds){
        _descImageInfos.push_back(vk::DescriptorImageInfo(s.sampler, s.textureView->getView(), s.layout));
        descWrites.push_back(vk::WriteDescriptorSet(_descSet,s.binding,0,1,s.descType,&_descImageInfos[_descImageInfos.size()-1],nullptr,nullptr));
    }
    vk_device.updateDescriptorSets(descWrites,nullptr);
}*/



void DescSetVK::setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset, int size){
    _uboBinds.emplace_back(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eUniformBuffer,offset,size<0?buffer.size():size);
}

void DescSetVK::setStorageBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset, int size){
	_uboBinds.emplace_back(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eStorageBuffer,offset,size<0?buffer.size():size);
}

void DescSetVK::setStorageTexture(const spTextureView &texture, const Sampler &sampler, size_t binding, const ShaderStage &stage) {
	auto internalTextureView = std::dynamic_pointer_cast<TextureViewVK>(texture);
	_samplerBinds.emplace_back(internalTextureView,createSampler(sampler),binding,shaderStageVK(stage),vk::DescriptorType::eStorageImage,vk::ImageLayout::eGeneral);
}

void DescSetVK::setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                           const ShaderStage &stage){
    auto internalTextureView = std::dynamic_pointer_cast<TextureViewVK>(texture);
    auto internalTexture = std::dynamic_pointer_cast<TextureVK>(texture->getTexture());
    _samplerBinds.emplace_back(internalTextureView,createSampler(sampler),binding,shaderStageVK(stage),vk::DescriptorType::eCombinedImageSampler,internalTexture->getImageLayout());
}

void DescSetVK::write() {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::vector<vk::WriteDescriptorSet>         descWrites;
	_descBufferInfos.resize(_uboBinds.size());
	for(auto u : _uboBinds){
		auto internalBuffer = std::dynamic_pointer_cast<BufferVK>(u.buffer.getBuffer());
		_descBufferInfos.emplace_back(internalBuffer->getVKBuffer(),u.offset,u.size);
		descWrites.emplace_back(_descSet,u.binding,0,1,u.descType,nullptr,&_descBufferInfos[_descBufferInfos.size()-1],nullptr);
	}
	_descImageInfos.resize(_samplerBinds.size());
	for(auto s : _samplerBinds){
		_descImageInfos.emplace_back(s.sampler, s.textureView->getView(), s.layout);
		descWrites.emplace_back(_descSet,s.binding,0,1,s.descType,&_descImageInfos[_descImageInfos.size()-1],nullptr,nullptr);
	}
	vk_device.updateDescriptorSets(descWrites,nullptr);
}

DescSetVK::UBOBinding::UBOBinding(const Uniform &_buffer, size_t _binding, const vk::ShaderStageFlags &_stage,
                                  const vk::DescriptorType &_descType, size_t _offset, size_t _size)
  : buffer(_buffer),binding(_binding),stage(_stage),descType(_descType), offset(_offset), size(_size)
  {}

DescSetVK::SamplerBinding::SamplerBinding(const spTextureViewVK& _textureView, const vk::Sampler &_sampler,
                                          size_t _binding, const vk::ShaderStageFlags &_stage,
                                          const vk::DescriptorType &_descType, const vk::ImageLayout &_layout)
  : textureView(_textureView), sampler(_sampler), binding(_binding), stage(_stage), descType(_descType), layout(_layout)
  {}

}
