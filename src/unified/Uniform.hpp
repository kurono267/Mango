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
        Uniform(const Uniform &u) : _gpu(u._gpu), _cpu(u._cpu), _is(u._is), _size(u._size) {}
        Uniform(const spBuffer& cpu, const spBuffer& gpu) : _gpu(gpu), _cpu(cpu), _size(cpu->size()),_is(true) {}

        virtual ~Uniform();

        void create(const size_t &size, BufferType type = BufferType::Uniform, const void *data = nullptr);
        void set(const size_t &size, const void *data);

        void update();

        spBuffer getBuffer();
        spBuffer getCPUBuffer();
        size_t size() const;

        operator bool() { return _is; }
    protected:
        spBuffer _gpu; // GPU buffer
        spBuffer _cpu; // CPU staging buffer

        bool _is = false;

        size_t _size;
};

}

#endif //MANGO_UNIFORM_HPP
