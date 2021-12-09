//
// Created by kurono267 on 09.12.2021.
//

#pragma once

#include "../GPUResources.hpp"
#include "MetalInit.hpp"

using namespace mango;

struct TextureMetal {
	TextureDesc desc;

	void* texture;
};

struct BufferMetal {
	void* buffer;
};

class MetalResources : public GPUResources {
	public:
		MetalResources(std::shared_ptr<MetalInit> metal);

		TextureID createTexture(const TextureDesc &desc) final;
		BufferID createBuffer(BufferType type, BufferMemory memory, size_t sizeInBytes) final;

		std::shared_ptr<MetalInit> metal;
	private:
		std::vector<TextureMetal> _textures;
		std::vector<BufferMetal> _buffers;
};



