//
// Created by kurono267 on 20.07.2021.
//

#pragma once

#include <cstdint>

namespace mango {

enum class BufferID : uint64_t {};

enum class BufferType : uint16_t {
		VertexBuffer = (1 << 0),
		IndexBuffer = (1 << 1),
		Uniform = (1 << 2),
		Storage = (1 << 3)
};

enum class BufferMemory : uint16_t {
		CPU,
		GPU
};

inline BufferType operator|(BufferType a, BufferType b) {
	return static_cast<BufferType>(static_cast<uint16_t>(a) | static_cast<uint16_t>(b));
}

struct BufferView {
	BufferView(const BufferID& id, uint32_t size, uint32_t offset = 0);
	BufferID buffer;
	uint32_t offset = 0;
	uint32_t size = 0;
};

}
