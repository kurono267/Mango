//
// Created by kurono267 on 20.07.2021.
//

#include "Buffer.hpp"

namespace mango {

BufferView::BufferView(const BufferID& id, uint32_t _size, uint32_t _offset) : buffer(id), offset(_offset), size(_size) {}

}
