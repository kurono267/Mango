//
// Created by kurono267 on 26.11.2018.
//

#include "Uniform.hpp"
#include "../api/Instance.hpp"

namespace mango {

Uniform::Uniform() : _is(false), _size(0) {}

Uniform::~Uniform(){}

void Uniform::create(const size_t& size, BufferType type,const void* data){
    _size = size;

    auto device = Instance::device();

    _buffer = device->createBuffer(type,MemoryType::DEVICE_HOST,size,(void*)data);


    _is = true;
}

void Uniform::set(const size_t& size,const void* data){
    if(_size != size)throw std::logic_error("Uniform failed: different size");
    else if(!data)throw std::logic_error("Data is nullptr");
    else if(_is){
    	_buffer->set(size,data);
    } else throw std::logic_error("Uniform failed: wrong set data, buffer don't exist");
}

spBuffer Uniform::getBuffer(){
    return _buffer;
}

size_t     Uniform::size() const {
    return _size;
}

}
