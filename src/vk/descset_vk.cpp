//
// Created by kurono267 on 26.11.2018.
//

#include "descset_vk.hpp"
#include "buffer_vk.hpp"
#include "texture_vk.hpp"
#include <set>

namespace mango::vulkan {

DescSetVK::DescSetVK(const spDevice &device) : _device(device) {}

void DescSetVK::create(){
    // Create Decription Set Layout
    std::vector<vk::DescriptorSetLayoutBinding> layoutBinds;
    std::vector<vk::DescriptorPoolSize>         poolSizes;

    auto vk_device = std::dynamic_pointer_cast<DeviceVK>(_device)->getDevice();

    std::unordered_map<int,int> typesDesc;
    // Add Layout binding for UBO
    for(auto u : _uboBinds){
        layoutBinds.push_back(vk::DescriptorSetLayoutBinding(u.binding,u.descType,1,u.stage));
        poolSizes.push_back(vk::DescriptorPoolSize(u.descType,1));
        if(typesDesc.find((int)u.descType) == typesDesc.end()){
            typesDesc.insert(std::pair<int,int>((int)u.descType,1));
        } else {
            typesDesc[(int)u.descType] += 1;
        }
    }
    for(auto s : _samplerBinds) {
        layoutBinds.push_back(
                vk::DescriptorSetLayoutBinding(s.binding, s.descType, 1, s.stage));
        poolSizes.push_back(vk::DescriptorPoolSize(s.descType, 1));
        if(typesDesc.find((int)s.descType) == typesDesc.end()){
            typesDesc.insert(std::pair<int,int>((int)s.descType,1));
        } else {
            typesDesc[(int)s.descType] += 1;
        }
    }
    /*int poolSize = 0;
    for(auto t : typesDesc){
        std::cout << to_string((vk::DescriptorType)t.first) << " " << t.second << std::endl;
        poolSizes.push_back(vk::DescriptorPoolSize((vk::DescriptorType)t.first,t.second));
        poolSize++;
    }*/

    vk::DescriptorSetLayoutCreateInfo layoutInfo(vk::DescriptorSetLayoutCreateFlags(),layoutBinds.size(),layoutBinds.data());
    _descLayout = vk_device.createDescriptorSetLayout(layoutInfo);

    vk::DescriptorPoolCreateInfo poolInfo(vk::DescriptorPoolCreateFlags(),1,poolSizes.size(),poolSizes.data());
    _descPool = vk_device.createDescriptorPool(poolInfo);

    vk::DescriptorSetAllocateInfo allocInfo(_descPool,1,&_descLayout);
    _descSet = vk_device.allocateDescriptorSets(allocInfo)[0];

    std::vector<vk::WriteDescriptorSet>         descWrites;
    _descBufferInfos.resize(_uboBinds.size());
    for(auto u : _uboBinds){
        auto internalBuffer = std::dynamic_pointer_cast<BufferVK>(u.buffer.getBuffer());
        _descBufferInfos.push_back(vk::DescriptorBufferInfo(internalBuffer->getVKBuffer(),0,u.buffer.size()));
        descWrites.push_back(vk::WriteDescriptorSet(_descSet,u.binding,0,1,u.descType,nullptr,&_descBufferInfos[_descBufferInfos.size()-1],nullptr));
    }
    _descImageInfos.resize(_samplerBinds.size());
    for(auto s : _samplerBinds){
        _descImageInfos.push_back(vk::DescriptorImageInfo(s.sampler, s.imageView, s.layout));
        descWrites.push_back(vk::WriteDescriptorSet(_descSet,s.binding,0,1,s.descType,&_descImageInfos[_descImageInfos.size()-1],nullptr,nullptr));
    }
    vk_device.updateDescriptorSets(descWrites,nullptr);
}

void DescSetVK::setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage){

    _uboBinds.emplace_back(buffer,binding,shaderStageVK(stage),vk::DescriptorType::eUniformBuffer);
}

void DescSetVK::setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                           const ShaderStage &stage){
    auto internalTexture = std::dynamic_pointer_cast<TextureViewVK>(texture);
    _samplerBinds.emplace_back(internalTexture->getView(),createSampler(_device,sampler),binding,shaderStageVK(stage),vk::DescriptorType::eCombinedImageSampler,vk::ImageLayout::eShaderReadOnlyOptimal);
}

void DescSetVK::release(spDevice device){
    auto vk_device = std::dynamic_pointer_cast<DeviceVK>(device)->getDevice();

    std::set<vk::Sampler> samplerSet;
    for(auto s : _samplerBinds){
        samplerSet.insert(s.sampler);
    }

    for(auto s : samplerSet){
        vk_device.destroySampler(s);
    }

    vk_device.destroyDescriptorSetLayout(_descLayout);
    vk_device.destroyDescriptorPool(_descPool);
}

    DescSetVK::UBOBinding::UBOBinding(const Uniform &_buffer, size_t _binding, const vk::ShaderStageFlags &_stage,
                                  const vk::DescriptorType &_descType)
  : buffer(_buffer),binding(_binding),stage(_stage),descType(_descType)
  {}

DescSetVK::SamplerBinding::SamplerBinding(const vk::ImageView &_imageView, const vk::Sampler &_sampler,
                                          size_t _binding, const vk::ShaderStageFlags &_stage,
                                          const vk::DescriptorType &_descType, const vk::ImageLayout &_layout)
  : imageView(_imageView), sampler(_sampler), binding(_binding), stage(_stage), descType(_descType), layout(_layout)
  {}

}
