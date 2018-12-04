//
// Created by kurono267 on 2018-12-04.
//

#include "utils.hpp"

namespace mango {

spTexture checkboardTexture(spDevice device, uint32_t width, uint32_t height, uint32_t step) {
    // Create data
    glm::vec4 *pixels = new glm::vec4[width * height];
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

    return device->createTexture(width,height,1,Format::R8G8B8A8Snorm,TextureType::Input,pixels);
}

}
