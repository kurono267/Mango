//
// Created by kurono267 on 2018-12-04.
//

#include "Utils.hpp"
#include "../api/Instance.hpp"
#include <random>

namespace mango {

spTexture checkboardTexture(uint32_t width, uint32_t height, uint32_t step) {
	auto device = Instance::device();
    // Create data
	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,width*height*sizeof(glm::vec4));
	auto pixels = (glm::vec4*)buffer->map();
    for (uint32_t y = 0; y < height; ++y) {
        uint32_t yStep = (y / step);
        bool isLine = (bool)(yStep % 2);
        for (uint32_t x = 0; x < width; ++x) {
            uint32_t xStep = (x / step);
            bool isX = (bool)((xStep + isLine) % 2);
            if (isX) {
                pixels[y * width + x] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
            } else pixels[y * width + x] = glm::vec4(0, 0, 0, 1.0f);
        }
    }
    buffer->unmap();

    spTexture texture = device->createTexture(width,height,1,Format::R32G32B32A32Sfloat,TextureUsage::TransferDst | TextureUsage::Sampled);
    texture->set(buffer);
    return texture;
}

spTexture createSinglePixelTexture(float value) {
	auto device = Instance::device();
	spTexture texture = device->createTexture(1,1,1,Format::R32Sfloat,TextureUsage::TransferDst | TextureUsage::Sampled);
	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,sizeof(float),(void*)&value);
	texture->set(buffer);
	return texture;
}

spTexture createSinglePixelTexture(const glm::vec2& value) {
	auto device = Instance::device();
	spTexture texture = device->createTexture(1,1,1,Format::R32G32Sfloat,TextureUsage::TransferDst | TextureUsage::Sampled);
	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,sizeof(glm::vec2),(void*)&value);
	texture->set(buffer);
	return texture;
}

spTexture createSinglePixelTexture(const glm::vec4 &value) {
	auto device = Instance::device();
	spTexture texture = device->createTexture(1,1,1,Format::R32G32B32A32Sfloat,TextureUsage::TransferDst | TextureUsage::Sampled);
	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,sizeof(glm::vec4),(void*)&value);
	texture->set(buffer);
	return texture;
}

spTexture createRandomTexture2D(const int width, const int height){
	spDevice device = Instance::device();
	spTexture texture = device->createTexture(width,height,1,Format::R32G32B32A32Sfloat,TextureUsage::TransferDst | TextureUsage::Sampled);

	spBuffer buffer = device->createBuffer(BufferType::CPU,MemoryType::HOST,width*height*sizeof(glm::vec4));
	glm::vec4* data = (glm::vec4*)buffer->map();

	std::default_random_engine generator;
	for(int i = 0;i<4;++i) {
		std::uniform_real_distribution<float> distribution(-1.f, 1.f);
		for (int y = 0; y < height; ++y) {
			for (int x = 0; x < width; ++x) {
				data[y * width + x][i] = distribution(generator);
			}
		}
	}
	buffer->unmap();

	texture->set(buffer);
	return texture;
}

}
