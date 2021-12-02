//
//  BufferRaw.cpp
//  MangoRenderer
//
//  Created by kurono267 on 08.05.2021.
//

#include "BufferRaw.hpp"

namespace mango {

BufferRaw::BufferRaw(size_t sizeInBytes) : _size(sizeInBytes) {
    _data = new uint8_t[sizeInBytes];
    _isOwned = true;
}

BufferRaw::BufferRaw(size_t sizeInBytes, uint8_t* bytes, bool isOwned)
    : _data(bytes), _size(sizeInBytes), _isOwned(isOwned) {}

BufferRaw::~BufferRaw() {
    if(_isOwned){
        delete[] _data;
    }
}

}
