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
			[[maybe_unused]] auto textureID = GPU::createTexture(desc);
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