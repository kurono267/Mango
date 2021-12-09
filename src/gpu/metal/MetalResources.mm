//
// Created by kurono267 on 09.12.2021.
//

#include "MetalResources.hpp"
#include "convert/MetalTextureTypes.hpp"

using namespace mango;

MetalResources::MetalResources(std::shared_ptr<MetalInit> _metal) : metal(_metal) {}

TextureID MetalResources::createTexture(const TextureDesc &desc) {
	id<MTLDevice> device = (id<MTLDevice>)metal->device;

	MTLTextureDescriptor* descriptor = [MTLTextureDescriptor new];
	descriptor.textureType = textureTypeMtl(desc.type);
	descriptor.pixelFormat = formatMetal(desc.format);
	descriptor.width = desc.width;
	descriptor.height = desc.height;
	descriptor.depth = desc.depth;
	descriptor.arrayLength = desc.layers;
	descriptor.mipmapLevelCount = desc.mipLevels;
	descriptor.storageMode = MTLStorageModePrivate;

	id<MTLTexture> texture = [device newTextureWithDescriptor:descriptor];
	if(!texture){
		throw std::runtime_error("Failed to create texture");
	}

	TextureMetal textureMetal;
	textureMetal.desc = desc;
	textureMetal.texture = texture;
	_textures.push_back(textureMetal);
	return (TextureID)(_textures.size()-1);
}

BufferID MetalResources::createBuffer(BufferType type, BufferMemory memory, size_t sizeInBytes) {
	BufferID result;
	return result;
}
