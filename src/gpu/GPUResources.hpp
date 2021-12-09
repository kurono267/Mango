//
// Created by kurono267 on 12.10.2021.
//

#pragma once

#include "general/TextureRaw.hpp"
#include "general/BufferRaw.hpp"
#include "general/Buffer.hpp"
#include <chrono>

using namespace std::chrono_literals;

namespace mango {

enum class TextureID : uint64_t {};

class GPUResources {
	public:
		virtual TextureID createTexture(const TextureDesc &desc) = 0;
		virtual BufferID createBuffer(BufferType type, BufferMemory memory, size_t sizeInBytes) = 0;
};

}
