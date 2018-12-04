//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_UNIFORM_HPP
#define MANGO_UNIFORM_HPP

#include "buffer.hpp"
#include "device.hpp"

namespace mango {

class Uniform {
    public:
        Uniform();
        Uniform(const Uniform &u) : _gpu(u._gpu), _cpu(u._cpu), _is(u._is), _size(u._size), _data(u._data) {}

        virtual ~Uniform();

        void create(spDevice device, const size_t &size, const void *data = nullptr);
        void set(const size_t &size, const void *data);

        spBuffer getBuffer();
        size_t size() const;

        operator bool() { return _is; }

    protected:
        spBuffer _gpu; // GPU buffer
        spBuffer _cpu; // CPU staging buffer

        bool _is = false;

        size_t _size;
        void *_data;
};

}

#endif //MANGO_UNIFORM_HPP
