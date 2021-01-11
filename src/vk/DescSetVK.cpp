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
		for(const auto& img : s.second.images){
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
    _uboBinds.emplace(binding,UBOBinding(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eUniformBuffer,offset,size<0?buffer.size():size));
}

void DescSetVK::setStorageBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset, int size){
	_uboBinds.emplace(binding,UBOBinding(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eStorageBuffer,offset,size<0?buffer.size():size));
}

void DescSetVK::setStorageTexture(const std::vector<spTextureView> &textures, const Sampler &sampler, size_t binding, const ShaderStage &stage) {
	auto samplerItr = _samplerBinds.find(binding);
	SamplerBinding samplerBinding(textures,sampler,binding,shaderStageVK(stage),vk::DescriptorType::eStorageImage);
	if(samplerItr != _samplerBinds.end()){
		auto vk_device = Instance::device<DeviceVK>()->getDevice();
		std::set<vk::Sampler> samplerSet;
		for(const auto& img : samplerItr->second.images){
			samplerSet.insert(img.sampler);
		}

		for(auto s : samplerSet){
			vk_device.destroySampler(s);
		}

		samplerItr->second = samplerBinding;
	} else _samplerBinds.emplace(binding,samplerBinding);
}

void DescSetVK::setStorageTexture(const spTextureView &texture, const Sampler &sampler, size_t binding, const ShaderStage &stage) {
	setStorageTexture(std::vector<spTextureView>({texture}),sampler,binding,stage);
}

void DescSetVK::setTexture(const std::vector<spTextureView>& textures, const Sampler& sampler, size_t binding, const ShaderStage& stage) {
	auto samplerItr = _samplerBinds.find(binding);
	SamplerBinding samplerBinding(textures,sampler,binding,shaderStageVK(stage),vk::DescriptorType::eCombinedImageSampler);
	if(samplerItr != _samplerBinds.end()){
		auto vk_device = Instance::device<DeviceVK>()->getDevice();
		std::set<vk::Sampler> samplerSet;
		for(const auto& img : samplerItr->second.images){
			samplerSet.insert(img.sampler);
		}

		for(auto s : samplerSet){
			vk_device.destroySampler(s);
		}

		samplerItr->second = samplerBinding;
	} else _samplerBinds.emplace(binding,samplerBinding);
}

void DescSetVK::setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                           const ShaderStage &stage){
    setTexture(std::vector<spTextureView>({texture}),sampler,binding,stage);
}

void DescSetVK::write() {
	auto vk_device = Instance::device<DeviceVK>()->getDevice();

	std::vector<vk::WriteDescriptorSet>         descWrites;
	_descBufferInfos.resize(_uboBinds.size());
	for(auto uItr : _uboBinds){
		auto u = uItr.second;
		auto internalBuffer = std::dynamic_pointer_cast<BufferVK>(u.buffer.getBuffer());
		_descBufferInfos.emplace_back(internalBuffer->getVKBuffer(),u.offset,u.size);
		descWrites.emplace_back(_descSet,u.binding,0,1,u.descType,nullptr,&_descBufferInfos[_descBufferInfos.size()-1],nullptr);
	}
	for(const auto& sItr : _samplerBinds){
		const auto& s = sItr.second;
		descWrites.emplace_back(_descSet,s.binding,0,s.images.size(),s.descType,s.images.data(),nullptr,nullptr);
	}
	vk_device.updateDescriptorSets(descWrites,nullptr);
}

DescSetVK::UBOBinding::UBOBinding(const Uniform &_buffer, size_t _binding, const vk::ShaderStageFlags &_stage,
                                  const vk::DescriptorType &_descType, size_t _offset, size_t _size)
  : buffer(_buffer),binding(_binding),stage(_stage),descType(_descType), offset(_offset), size(_size)
  {}

DescSetVK::SamplerBinding::SamplerBinding(const std::vector<spTextureView>& _views, const Sampler& _sampler, size_t _binding,
						  const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType)
  : views(_views),binding(_binding), stage(_stage), descType(_descType)
  {
	for(int i = 0;i<views.size();++i){
		auto view = std::dynamic_pointer_cast<TextureViewVK>(views[i]);
		auto texture = std::dynamic_pointer_cast<TextureVK>(view->getTexture());
		images.emplace_back(createSampler(_sampler), view->getView(), imageLayoutVK(texture->layout()));
	}
  }

}
