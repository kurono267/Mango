//
// Created by kurono267 on 28.06.2021.
//

#pragma once

#include "../../general/TextureTypes.hpp"
#include <vulkan/vulkan.hpp>
#include "../../general/Texture.hpp"

using namespace mango;

struct TextureTypeVK {
	vk::ImageType image;
	vk::ImageViewType view;
};

vk::Format formatVK(const Format &format);
Format formatVK2Mango(const vk::Format &format);

TextureTypeVK textureTypeVK(const TextureType& type);

vk::SamplerAddressMode samplerAddressModeVK(const SamplerAddressMode &samplerAddressMode);
vk::SamplerMipmapMode samplerMipmapModeVK(const SamplerMipmapMode &mipmapMode);
vk::Filter filterVK(const Filter &filter);
vk::BorderColor borderColorVK(const BorderColor &borderColor);

vk::Sampler samplerVK(vk::Device& device, const Sampler& sampler);