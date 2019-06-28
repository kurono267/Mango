//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_DESCSET_VK_HPP
#define MANGO_DESCSET_VK_HPP

#include "../api/descset.hpp"
#include "default.hpp"
#include "device_vk.hpp"

namespace mango::vulkan {

class DescSetVK : public mango::DescSet {
public:
    DescSetVK(const spDevice& device);
    ~DescSetVK() override;

    void create() override;

    void setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage) override;

    void setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                    const ShaderStage &stage) override;

    void release(spDevice device) override;

    vk::DescriptorSet getSet(){return _descSet;}
    vk::DescriptorSetLayout getLayout(){return _descLayout;}
private:
    spDevice _device;

    vk::DescriptorSetLayout _descLayout;
    vk::DescriptorSet       _descSet;
    vk::DescriptorPool      _descPool;

    std::vector<vk::DescriptorImageInfo>  _descImageInfos;
    std::vector<vk::DescriptorBufferInfo> _descBufferInfos;

    struct UBOBinding {
        UBOBinding(const Uniform& _buffer, size_t _binding, const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType);
        Uniform buffer;
        size_t    binding;
        vk::ShaderStageFlags stage;
        vk::DescriptorType   descType;
    };
    struct SamplerBinding {
        SamplerBinding(const vk::ImageView& _imageView, const vk::Sampler& _sampler, size_t _binding,
                const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType, const vk::ImageLayout& _layout);
        vk::ImageView imageView;
        vk::Sampler   sampler;
        size_t    binding;
        vk::ShaderStageFlags stage;
        vk::DescriptorType   descType;
        vk::ImageLayout      layout;
    };
    std::vector<UBOBinding>     _uboBinds;
    std::vector<SamplerBinding> _samplerBinds;
};

}

#endif //MANGO_DESCSET_VK_HPP
