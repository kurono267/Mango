//
// Created by kurono267 on 28.06.2021.
//

#include "VkTextureTypes.hpp"
#include "VkRenderPipelineTypes.hpp"

using namespace mango;

vk::Format formatVK(const Format &format) {
	switch (format) {
		case Format::Undefined:
			return vk::Format::eUndefined;
		case Format::R8Unorm:
			return vk::Format::eR8Unorm;
		case Format::R8Snorm:
			return vk::Format::eR8Snorm;
		case Format::R8Uint:
			return vk::Format::eR8Uint;
		case Format::R8Sint:
			return vk::Format::eR8Sint;
		case Format::R8G8Unorm:
			return vk::Format::eR8G8Unorm;
		case Format::R8G8Snorm:
			return vk::Format::eR8G8Snorm;
		case Format::R8G8Uint:
			return vk::Format::eR8G8Uint;
		case Format::R8G8Sint:
			return vk::Format::eR8G8Sint;
		case Format::R8G8B8A8Unorm:
			return vk::Format::eR8G8B8A8Unorm;
		case Format::R8G8B8A8Snorm:
			return vk::Format::eR8G8B8A8Snorm;
		case Format::R8G8B8A8Uint:
			return vk::Format::eR8G8B8A8Uint;
		case Format::R8G8B8A8Sint:
			return vk::Format::eR8G8B8A8Sint;
		case Format::R8G8B8A8Srgb:
			return vk::Format::eR8G8B8A8Srgb;
		case Format::B8G8R8A8Unorm:
			return vk::Format::eB8G8R8A8Unorm;
		case Format::B8G8R8A8Srgb:
			return vk::Format::eB8G8R8A8Srgb;
		case Format::A2R10G10B10UnormPack32:
			return vk::Format::eA2R10G10B10UnormPack32;
		case Format::A2R10G10B10UintPack32:
			return vk::Format::eA2R10G10B10UintPack32;
		case Format::A2B10G10R10UnormPack32:
			return vk::Format::eA2B10G10R10UnormPack32;
		case Format::R16Unorm:
			return vk::Format::eR16Unorm;
		case Format::R16Snorm:
			return vk::Format::eR16Snorm;
		case Format::R16Uint:
			return vk::Format::eR16Uint;
		case Format::R16Sint:
			return vk::Format::eR16Sint;
		case Format::R16Sfloat:
			return vk::Format::eR16Sfloat;
		case Format::R16G16Unorm:
			return vk::Format::eR16G16Unorm;
		case Format::R16G16Snorm:
			return vk::Format::eR16G16Snorm;
		case Format::R16G16Uint:
			return vk::Format::eR16G16Uint;
		case Format::R16G16Sint:
			return vk::Format::eR16G16Sint;
		case Format::R16G16Sfloat:
			return vk::Format::eR16G16Sfloat;
		case Format::R16G16B16A16Unorm:
			return vk::Format::eR16G16B16A16Unorm;
		case Format::R16G16B16A16Snorm:
			return vk::Format::eR16G16B16A16Snorm;
		case Format::R16G16B16A16Uint:
			return vk::Format::eR16G16B16A16Uint;
		case Format::R16G16B16A16Sint:
			return vk::Format::eR16G16B16A16Sint;
		case Format::R16G16B16A16Sfloat:
			return vk::Format::eR16G16B16A16Sfloat;
		case Format::R32Uint:
			return vk::Format::eR32Uint;
		case Format::R32Sint:
			return vk::Format::eR32Sint;
		case Format::R32Sfloat:
			return vk::Format::eR32Sfloat;
		case Format::R32G32Uint:
			return vk::Format::eR32G32Uint;
		case Format::R32G32Sint:
			return vk::Format::eR32G32Sint;
		case Format::R32G32Sfloat:
			return vk::Format::eR32G32Sfloat;
		case Format::R32G32B32A32Uint:
			return vk::Format::eR32G32B32A32Uint;
		case Format::R32G32B32A32Sint:
			return vk::Format::eR32G32B32A32Sint;
		case Format::R32G32B32A32Sfloat:
			return vk::Format::eR32G32B32A32Sfloat;
		case Format::D16Unorm:
			return vk::Format::eD16Unorm;
		case Format::D16UnormS8Uint:
			return vk::Format::eD16UnormS8Uint;
		case Format::D24UnormS8Uint:
			return vk::Format::eD24UnormS8Uint;
		case Format::D32Sfloat:
			return vk::Format::eD32Sfloat;
		case Format::D32SfloatS8Uint:
			return vk::Format::eD32SfloatS8Uint;
	}
}

Format formatVK2Mango(const vk::Format &format) {
	switch (format) {
		case vk::Format::eR8Unorm:
			return Format::R8Unorm;
		case vk::Format::eR8Snorm:
			return Format::R8Snorm;
		case vk::Format::eR8Uint:
			return Format::R8Uint;
		case vk::Format::eR8Sint:
			return Format::R8Sint;
		case vk::Format::eR8G8Unorm:
			return Format::R8G8Unorm;
		case vk::Format::eR8G8Snorm:
			return Format::R8G8Snorm;
		case vk::Format::eR8G8Uint:
			return Format::R8G8Uint;
		case vk::Format::eR8G8Sint:
			return Format::R8G8Sint;
		case vk::Format::eR8G8B8A8Unorm:
			return Format::R8G8B8A8Unorm;
		case vk::Format::eR8G8B8A8Snorm:
			return Format::R8G8B8A8Snorm;
		case vk::Format::eR8G8B8A8Uint:
			return Format::R8G8B8A8Uint;
		case vk::Format::eR8G8B8A8Sint:
			return Format::R8G8B8A8Sint;
		case vk::Format::eR8G8B8A8Srgb:
			return Format::R8G8B8A8Srgb;
		case vk::Format::eB8G8R8A8Unorm:
			return Format::B8G8R8A8Unorm;
		case vk::Format::eB8G8R8A8Srgb:
			return Format::B8G8R8A8Srgb;
		case vk::Format::eA2R10G10B10UnormPack32:
			return Format::A2R10G10B10UnormPack32;
		case vk::Format::eA2R10G10B10UintPack32:
			return Format::A2R10G10B10UintPack32;
		case vk::Format::eA2B10G10R10UnormPack32:
			return Format::A2B10G10R10UnormPack32;
		case vk::Format::eR16Unorm:
			return Format::R16Unorm;
		case vk::Format::eR16Snorm:
			return Format::R16Snorm;
		case vk::Format::eR16Uint:
			return Format::R16Uint;
		case vk::Format::eR16Sint:
			return Format::R16Sint;
		case vk::Format::eR16Sfloat:
			return Format::R16Sfloat;
		case vk::Format::eR16G16Unorm:
			return Format::R16G16Unorm;
		case vk::Format::eR16G16Snorm:
			return Format::R16G16Snorm;
		case vk::Format::eR16G16Uint:
			return Format::R16G16Uint;
		case vk::Format::eR16G16Sint:
			return Format::R16G16Sint;
		case vk::Format::eR16G16Sfloat:
			return Format::R16G16Sfloat;
		case vk::Format::eR16G16B16A16Unorm:
			return Format::R16G16B16A16Unorm;
		case vk::Format::eR16G16B16A16Snorm:
			return Format::R16G16B16A16Snorm;
		case vk::Format::eR16G16B16A16Uint:
			return Format::R16G16B16A16Uint;
		case vk::Format::eR16G16B16A16Sint:
			return Format::R16G16B16A16Sint;
		case vk::Format::eR16G16B16A16Sfloat:
			return Format::R16G16B16A16Sfloat;
		case vk::Format::eR32Uint:
			return Format::R32Uint;
		case vk::Format::eR32Sint:
			return Format::R32Sint;
		case vk::Format::eR32Sfloat:
			return Format::R32Sfloat;
		case vk::Format::eR32G32Uint:
			return Format::R32G32Uint;
		case vk::Format::eR32G32Sint:
			return Format::R32G32Sint;
		case vk::Format::eR32G32Sfloat:
			return Format::R32G32Sfloat;
		case vk::Format::eR32G32B32A32Uint:
			return Format::R32G32B32A32Uint;
		case vk::Format::eR32G32B32A32Sint:
			return Format::R32G32B32A32Sint;
		case vk::Format::eR32G32B32A32Sfloat:
			return Format::R32G32B32A32Sfloat;
		case vk::Format::eD16Unorm:
			return Format::D16Unorm;
		case vk::Format::eD16UnormS8Uint:
			return Format::D16UnormS8Uint;
		case vk::Format::eD24UnormS8Uint:
			return Format::D24UnormS8Uint;
		case vk::Format::eD32Sfloat:
			return Format::D32Sfloat;
		case vk::Format::eD32SfloatS8Uint:
			return Format::D32SfloatS8Uint;
			// Default case
		case vk::Format::eUndefined:
		default:
			return Format::Undefined;
	}
}

vk::SamplerAddressMode samplerAddressModeVK(const SamplerAddressMode &samplerAddressMode) {
	switch (samplerAddressMode) {
		case SamplerAddressMode::Repeat:
			return vk::SamplerAddressMode::eRepeat;
			case SamplerAddressMode::MirroredRepeat:
				return vk::SamplerAddressMode::eMirroredRepeat;
				case SamplerAddressMode::ClampToEdge:
					return vk::SamplerAddressMode::eClampToEdge;
					case SamplerAddressMode::ClampToBorder:
						return vk::SamplerAddressMode::eClampToBorder;
						case SamplerAddressMode::MirrorClampToEdge:
							return vk::SamplerAddressMode::eMirrorClampToEdge;
	}
}

vk::SamplerMipmapMode samplerMipmapModeVK(const SamplerMipmapMode &mipmapMode) {
	switch (mipmapMode) {
		case SamplerMipmapMode::Nearest:
			return vk::SamplerMipmapMode::eNearest;
			case SamplerMipmapMode::Linear:
				return vk::SamplerMipmapMode::eLinear;
	}
}

vk::Filter filterVK(const Filter &filter) {
	switch (filter) {
		case Filter::Nearest:
			return vk::Filter::eNearest;
			case Filter::Linear:
				return vk::Filter::eLinear;
				case Filter::Cubic:
					return vk::Filter::eCubicIMG;
	}
}

vk::BorderColor borderColorVK(const BorderColor &borderColor) {
	switch (borderColor) {
		case BorderColor::FloatTransparentBlack:
			return vk::BorderColor::eFloatTransparentBlack;
			case BorderColor::IntTransparentBlack:
				return vk::BorderColor::eIntTransparentBlack;
				case BorderColor::FloatOpaqueBlack:
					return vk::BorderColor::eFloatOpaqueBlack;
					case BorderColor::IntOpaqueBlack:
						return vk::BorderColor::eIntOpaqueBlack;
						case BorderColor::FloatOpaqueWhite:
							return vk::BorderColor::eFloatOpaqueWhite;
							case BorderColor::IntOpaqueWhite:
								return vk::BorderColor::eIntOpaqueWhite;
	}
}

vk::Sampler samplerVK(vk::Device& device, const Sampler& sampler){
	vk::SamplerCreateInfo createInfo;
	createInfo.addressModeU = samplerAddressModeVK(sampler.addressModeU);
	createInfo.addressModeV = samplerAddressModeVK(sampler.addressModeV);
	createInfo.addressModeW = samplerAddressModeVK(sampler.addressModeW);
	createInfo.anisotropyEnable = (VkBool32)sampler.anisotropyEnable;
	createInfo.borderColor = borderColorVK(sampler.borderColor);
	createInfo.compareEnable = (VkBool32)sampler.compareEnable;
	createInfo.compareOp = compareOpVK(sampler.compareOp);
	createInfo.magFilter = filterVK(sampler.magFilter);
	createInfo.minFilter = filterVK(sampler.minFilter);
	createInfo.maxAnisotropy = sampler.maxAnisotropy;
	createInfo.maxLod = sampler.maxLod;
	createInfo.minLod = sampler.minLod;
	createInfo.mipLodBias = sampler.mipLodBias;
	createInfo.mipmapMode = samplerMipmapModeVK(sampler.mipmapMode);
	return device.createSampler(createInfo);
}
