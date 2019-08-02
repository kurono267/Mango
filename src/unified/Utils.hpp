//
// Created by kurono267 on 2018-12-04.
//

#ifndef MANGO_UNIFIED_UTILS_HPP
#define MANGO_UNIFIED_UTILS_HPP

#include "api/Texture.hpp"

namespace mango {

spTexture checkboardTexture(spDevice device,uint32_t width, uint32_t height, uint32_t step);
spTexture createSinglePixelTexture(spDevice device, float value);
spTexture createSinglePixelTexture(spDevice device, const glm::vec4& value);

}

#endif //MANGO_UTILS_HPP
