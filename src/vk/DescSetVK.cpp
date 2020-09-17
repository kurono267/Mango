//
// Created by kurono267 on 26.11.2018.
//

#include "DescSetVK.hpp"
#include "BufferVK.hpp"
#include "TextureVK.hpp"
#include <set>
#include <iostream>
#include "../api/Instance.hpp"
#include "DescPoolVK.hpp"

namespace mango::vulkan {

DescSetVK::DescSetVK() {}

DescSetVK::DescSetVK(const vk::DescriptorSet &descSet, const spDescPool& pool) : _descSet(descSet), _descPool(pool) {

}

DescSetVK::~DescSetVK() {
	std::cout << "~DescSetVK" << std::endl;
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::set<vk::Sampler> samplerSet;
	for(const auto& s : _samplerBinds){
		for(const auto& img : s.images){
			samplerSet.insert(img.sampler);
		}
	}

	for(auto s : samplerSet){
		vk_device.destroySampler(s);
	}

	auto pool = std::dynamic_pointer_cast<DescPoolVK>(_descPool);
	vk_device.freeDescriptorSets(pool->getPool(),1,&_descSet);
}

void DescSetVK::setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset, int size){
    _uboBinds.emplace_back(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eUniformBuffer,offset,size<0?buffer.size():size);
}

void DescSetVK::setStorageBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset, int size){
	_uboBinds.emplace_back(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eStorageBuffer,offset,size<0?buffer.size():size);
}

void DescSetVK::setStorageTexture(const spTextureView &texture, const Sampler &sampler, size_t binding, const ShaderStage &stage) {
	auto internalTextureView = std::dynamic_pointer_cast<TextureViewVK>(texture);
	auto internalTexture = std::dynamic_pointer_cast<TextureVK>(texture->getTexture());
	std::vector<vk::DescriptorImageInfo> images = {vk::DescriptorImageInfo(createSampler(sampler), internalTextureView->getView(), imageLayoutVK(internalTexture->layout()))};
	_samplerBinds.emplace_back(images,binding,shaderStageVK(stage),vk::DescriptorType::eStorageImage);
}

void DescSetVK::setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                           const ShaderStage &stage){
    auto internalTextureView = std::dynamic_pointer_cast<TextureViewVK>(texture);
    auto internalTexture = std::dynamic_pointer_cast<TextureVK>(texture->getTexture());
	std::vector<vk::DescriptorImageInfo> images = {vk::DescriptorImageInfo(createSampler(sampler), internalTextureView->getView(), imageLayoutVK(internalTexture->layout()))};
	SamplerBinding samplerBinding(images,binding,shaderStageVK(stage),vk::DescriptorType::eCombinedImageSampler);
	samplerBinding.textureViews = {internalTextureView};
    _samplerBinds.emplace_back(samplerBinding);
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
	for(const auto& s : _samplerBinds){
		descWrites.emplace_back(_descSet,s.binding,0,s.images.size(),s.descType,s.images.data(),nullptr,nullptr);
	}
	vk_device.updateDescriptorSets(descWrites,nullptr);
}

DescSetVK::UBOBinding::UBOBinding(const Uniform &_buffer, size_t _binding, const vk::ShaderStageFlags &_stage,
                                  const vk::DescriptorType &_descType, size_t _offset, size_t _size)
  : buffer(_buffer),binding(_binding),stage(_stage),descType(_descType), offset(_offset), size(_size)
  {}

DescSetVK::SamplerBinding::SamplerBinding(const std::vector<vk::DescriptorImageInfo>& _images, size_t _binding,
						  const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType)
  : images(_images), binding(_binding), stage(_stage), descType(_descType)
  {}

}
