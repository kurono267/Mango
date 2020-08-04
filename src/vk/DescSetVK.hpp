//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_DESCSETVK_HPP
#define MANGO_DESCSETVK_HPP

#include "../api/DescSet.hpp"
#include "default.hpp"
#include "DeviceVK.hpp"
#include "TextureVK.hpp"

namespace mango::vulkan {

class DescSetVK : public mango::DescSet {
public:
    DescSetVK();
    ~DescSetVK() override;

    void create() override;

    void setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset = 0, int size = -1) override;
    void setStorageBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset = 0, int size = -1) override;

    void setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                    const ShaderStage &stage) override;
	void setStorageTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
			        const ShaderStage &stage) override;

    vk::DescriptorSet getSet(){return _descSet;}
    vk::DescriptorSetLayout getLayout(){return _descLayout;}
private:
    vk::DescriptorSetLayout _descLayout;
    vk::DescriptorSet       _descSet;
    vk::DescriptorPool      _descPool;

    std::vector<vk::DescriptorImageInfo>  _descImageInfos;
    std::vector<vk::DescriptorBufferInfo> _descBufferInfos;

    struct UBOBinding {
        UBOBinding(const Uniform& _buffer, size_t _binding, const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType, size_t offset, size_t size);
        Uniform buffer;
        size_t    binding;
        vk::ShaderStageFlags stage;
        vk::DescriptorType   descType;
		size_t offset;
        size_t size;
    };
    struct SamplerBinding {
        SamplerBinding(const spTextureViewVK& _textureView, const vk::Sampler& _sampler, size_t _binding,
                const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType, const vk::ImageLayout& _layout);
		spTextureViewVK textureView;
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

#endif //MANGO_DESCSETVK_HPP
