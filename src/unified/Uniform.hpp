//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_UNIFORM_HPP
#define MANGO_UNIFORM_HPP

#include "../api/Buffer.hpp"
#include "../api/Device.hpp"

namespace mango {

class Uniform {
    public:
        Uniform();
        Uniform(const Uniform &u) : _buffer(u._buffer), _is(u._is), _size(u._size) {}

        virtual ~Uniform();

        void create(const size_t &size, BufferType type = BufferType::Uniform, const void *data = nullptr);
        void set(const size_t &size, const void *data);

        spBuffer getBuffer();
        size_t size() const;

        operator bool() { return _is; }
    protected:
        spBuffer _buffer; // GPU buffer
        void* _data;

        bool _is = false;

        size_t _size;
};

}

#endif //MANGO_UNIFORM_HPP
