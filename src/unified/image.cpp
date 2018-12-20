//
// Created by kurono267 on 2018-12-21.
//

#include "image.hpp"
#include <iostream>
#include <stbi/stb_image.h>

inline spImage4b loadImage(const std::string& filename){
    int width,height,channels;
    uint8_t *data = stbi_load(filename.c_str(), &width, &height, &channels, 4); // we want RGBA always
    if (!data) {
        std::cout << "Can't load resource image " << filename << std::endl;
        return nullptr;
    }

    spImage4b image = std::make_shared<Image4b>(glm::ivec2(width,height));
    memcpy((void*)image->data().data(), (void*)data, static_cast<size_t>(width * height * 4));

    stbi_image_free((void *) data);
    return image;
}