//
//  Texture.cpp
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//

#include "Texture.hpp"

namespace mango {

TextureLayout::TextureLayout(TextureType type_, uint32_t width_, uint32_t height_, uint32_t depth_, Format format_, uint32_t mipLevels_, uint32_t layers_)
    : type(type_),width(width_), height(height_), depth(depth_), format(format_), mipLevels(mipLevels_), layers(layers_) {}

size_t bufferSize(const TextureLayout& layout) {
    size_t size = 0;
    uint32_t w = layout.width;
    uint32_t h = layout.height;
    uint32_t d = layout.depth;
    auto pixel = pixelSize(layout.format);
    for(int level = 0;level<layout.mipLevels;++level){
        size += w*h*d*layout.layers*pixel;
        w /= 2;
        h /= 2;
        d /= 2;
    }
    return size;
}

}
