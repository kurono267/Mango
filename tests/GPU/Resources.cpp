//
// Created by kurono267 on 09.12.2021.
//

#include "Resources.hpp"

using namespace mango;

const int texSize = 32;
const int layers = 10;

void textureCreateTest() {
	try {
		// Create 1D texture
		{
			TextureDesc desc(TextureType::Texture1D, texSize, 1, 1, Format::R8G8B8A8Unorm, 1, 1);
			auto textureID = GPU::createTexture(desc);
		}
		// Create 2D texture
		{
			TextureDesc desc(TextureType::Texture2D, texSize, texSize, 1, Format::R8G8B8A8Unorm, 1, 1);
			auto textureID = GPU::createTexture(desc);
		}
		// Create 3D texture
		{
			TextureDesc desc(TextureType::Texture3D, texSize, texSize, texSize, Format::R8G8B8A8Unorm, 1, 1);
			auto textureID = GPU::createTexture(desc);
		}
		// Create 1D array
		{
			TextureDesc desc(TextureType::Texture1DArray, texSize, 1, 1, Format::R8G8B8A8Unorm, 1, layers);
			auto textureID = GPU::createTexture(desc);
		}
		// Create 2D array
		{
			TextureDesc desc(TextureType::Texture2DArray, texSize, texSize, 1, Format::R8G8B8A8Unorm, 1, layers);
			auto textureID = GPU::createTexture(desc);
		}
		// Create cubemap
		{
			TextureDesc desc(TextureType::TextureCube, texSize, texSize, 1, Format::R8G8B8A8Unorm, 1, 1);
			auto textureID = GPU::createTexture(desc);
		}
		// Create cubemap array
		{
			TextureDesc desc(TextureType::TextureCubeArray, texSize, texSize, 1, Format::R8G8B8A8Unorm, 1, layers);
			auto textureID = GPU::createTexture(desc);
		}
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		ASSERT_FALSE(1);
	}
}

const int bufSize = 1024;

void bufferCreateTest() {
	try {
		// Create IndexBuffer with CPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::IndexBuffer, BufferMemory::CPU,bufSize);
		}
		// Create IndexBuffer with GPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::IndexBuffer, BufferMemory::GPU,bufSize);
		}
		// Create VertexBuffer with CPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::VertexBuffer, BufferMemory::CPU,bufSize);
		}
		// Create VertexBuffer with GPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::VertexBuffer, BufferMemory::GPU,bufSize);
		}
		// Create StorageBuffer with CPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::Storage, BufferMemory::CPU,bufSize);
		}
		// Create StorageBuffer with GPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::Storage, BufferMemory::GPU,bufSize);
		}
		// Create UniformBuffer with CPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::Uniform, BufferMemory::CPU,bufSize);
		}
		// Create UniformBuffer with GPU Memory
		{
			auto bufferID = GPU::createBuffer(BufferType::Uniform, BufferMemory::GPU,bufSize);
		}
	} catch (const std::exception& e){
		std::cerr << e.what() << std::endl;
		ASSERT_FALSE(1);
	}
}