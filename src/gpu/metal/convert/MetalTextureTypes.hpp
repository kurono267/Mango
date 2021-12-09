//
// Created by kurono267 on 09.12.2021.
//

#pragma once

#include "../../general/TextureTypes.hpp"
#include <Metal/Metal.h>
#include "../../general/Texture.hpp"

using namespace mango;

MTLPixelFormat formatMetal(const Format &format);
Format formatMetal2Mango(const MTLPixelFormat &format);

MTLTextureType textureTypeMtl(const TextureType& type);

//vk::SamplerAddressMode samplerAddressModeVK(const SamplerAddressMode &samplerAddressMode);
//vk::SamplerMipmapMode samplerMipmapModeVK(const SamplerMipmapMode &mipmapMode);
//vk::Filter filterVK(const Filter &filter);
//vk::BorderColor borderColorVK(const BorderColor &borderColor);

//vk::Sampler samplerVK(vk::Device& device, const Sampler& sampler);


