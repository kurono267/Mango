//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_DESCSETVK_HPP
#define MANGO_DESCSETVK_HPP

#include "../api/DescSet.hpp"
#include "default.hpp"
#include "DeviceVK.hpp"
#include "TextureVK.hpp"
#include "../api/DescPool.hpp"
#include "DescLayoutVK.hpp"

namespace mango::vulkan {

class DescSetVK : public mango::DescSet {
public:
    DescSetVK();
    DescSetVK(const vk::DescriptorSet& descSet, const vk::DescriptorSetLayout& layout);
    ~DescSetVK() override;

    void setUniformBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset = 0, int size = -1) override;
    void setStorageBuffer(const Uniform &buffer, size_t binding, const ShaderStage &stage, size_t offset = 0, int size = -1) override;

	void setTexture(const std::vector<spTextureView>& textures, const Sampler& sampler, size_t binding, const ShaderStage& stage) final;
	void setStorageTexture(const std::vector<spTextureView> &texture, const Sampler &sampler, size_t binding, const ShaderStage &stage) final;

    void setTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
                    const ShaderStage &stage) override;
	void setStorageTexture(const spTextureView &texture, const Sampler &sampler, size_t binding,
			        const ShaderStage &stage) override;

	void write() final;

    vk::DescriptorSet getSet(){return _descSet;}
    vk::DescriptorSetLayout getLayout(){
    	return _layout;
    }
private:
    vk::DescriptorSet       _descSet;
    vk::DescriptorSetLayout _layout;

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
        SamplerBinding(const std::vector<spTextureView>& _views, const Sampler& _sampler, size_t _binding,
                const vk::ShaderStageFlags& _stage, const vk::DescriptorType& _descType);
        size_t    binding;
        vk::ShaderStageFlags stage;
        vk::DescriptorType   descType;
        std::vector<vk::DescriptorImageInfo> images;
        std::vector<spTextureView> views;
    };
    std::unordered_map<int,UBOBinding>     _uboBinds;
    std::unordered_map<int,SamplerBinding> _samplerBinds;
};

}

#endif //MANGO_DESCSETVK_HPP
