//
// Created by kurono267 on 2018-12-04.
//

#ifndef MANGO_UNIFIED_UTILS_HPP
#define MANGO_UNIFIED_UTILS_HPP

#include "../api/Texture.hpp"

namespace mango {

spTexture checkboardTexture(uint32_t width, uint32_t height, uint32_t step);
spTexture createSinglePixelTexture(float value);
spTexture createSinglePixelTexture(const glm::vec2& value);
spTexture createSinglePixelTexture(const glm::vec4& value);

spTexture createRandomTexture2D(const int width, const int height);

Format formatByComponentsDepth(int components,int depth,bool srgb);

}

#endif //MANGO_UTILS_HPP
