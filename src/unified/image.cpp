//
// Created by kurono267 on 2018-12-21.
//

#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stbi/stb_image.h>

#include "Image.hpp"

spImage4b loadImage(const std::string& filename){
    int width,height,channels;
    uint8_t *data = stbi_load(filename.c_str(), &width, &height, &channels, 4); // we want RGBA always
    if (!data) {
        std::cout << "Can't load resource image " << filename << std::endl;
        return nullptr;
    }

    spImage4b image = std::make_shared<Image4b>(glm::ivec2(width,height));
    memcpy((void*)image->data().data(), (void*)data, static_cast<size_t>(width * height * 4));

    stbi_image_free((void *) data);

    generateMipMaps(*image);
    return image;
}

size_t computeImageSizeWithMips(const glm::ivec2 &size, size_t mipLevels, std::vector<ImageLOD>& lodOffsets) {
    glm::ivec2 currSize = size;
    size_t currOffset = 0;
    size_t currSizeElements = 0;
    lodOffsets.resize(mipLevels);
    for(int mip = 0;mip<mipLevels;++mip){
        ImageLOD lod;
        lod.offset = currOffset;
        lod.size = currSize;
        lodOffsets[mip] = lod;
        currSizeElements += currSize.x*currSize.y;

        currSize /= 2;
        currOffset = currSizeElements;
    }
    return currSizeElements;
}

size_t computeMaxMipLevels(const glm::ivec2 &size) {
    return (size_t) std::min(log2((float)size.x), log2((float)size.y));
}

spImage4f loadImageHDRI(const std::string &filename, bool clamp) {
	int width,height,channels;
	stbi_loadf(filename.c_str(),&width, &height, &channels, 4);
	float *data = stbi_loadf(filename.c_str(),&width, &height, &channels, 4); // we want RGBA always
	if (!data) {
		std::cout << "Can't load resource image " << filename << std::endl;
		return nullptr;
	}

	spImage4f image = std::make_shared<Image4f>(glm::ivec2(width,height));
	memcpy((void*)image->data().data(), (void*)data, static_cast<size_t>(width * height * 4 * sizeof(float)));

	stbi_image_free((void *) data);

	if(clamp){
		for(int y = 0;y<image->height();++y){
			for(int x = 0;x<image->width();++x){
				(*image)(x,y) = glm::clamp((*image)(x,y),glm::vec4(0.0f),glm::vec4(1.0f));
			}
		}
	}

	generateMipMaps(*image);
	return image;
}
