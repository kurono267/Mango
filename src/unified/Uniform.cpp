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

    _cpu = device->createBuffer(type,MemoryType::HOST,size);
    _gpu = device->createBuffer(type,MemoryType::DEVICE,size);

    _is = true;

    if(data)set(size,data);
}

void Uniform::set(const size_t& size,const void* data){
    if(_size != size)throw std::logic_error("Uniform failed: different size");
    else if(!data)throw std::logic_error("Data is nullptr");
    else if(_is){
        _cpu->set(size,data);
        _cpu->copy(_gpu);
    } else throw std::logic_error("Uniform failed: wrong set data, buffer don't exist");
}

void Uniform::update() {
	_cpu->copy(_gpu);
}

spBuffer Uniform::getBuffer(){
    return _gpu;
}

spBuffer Uniform::getCPUBuffer() {
	return _cpu;
}

size_t     Uniform::size() const {
    return _size;
}

}
