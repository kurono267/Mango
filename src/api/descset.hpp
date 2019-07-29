//
// Created by kurono267 on 26.11.2018.
//

#ifndef MANGO_DESCSET_HPP
#define MANGO_DESCSET_HPP

#include <memory>
#include "types.hpp"
#include "texture.hpp"
#include "unified/uniform.hpp"

namespace mango {

class Device;
typedef std::shared_ptr<Device> spDevice;

class DescSet {
public:
    DescSet() = default;
    virtual ~DescSet() = default;

    virtual void create() = 0;

    virtual void setUniformBuffer(const Uniform& buffer,size_t binding,const ShaderStage& stage) = 0;
    virtual void setTexture(const spTextureView& texture,const Sampler& sampler, size_t binding, const ShaderStage& stage) = 0;
};

typedef std::shared_ptr<DescSet> spDescSet;

}

#endif //MANGO_DESCSET_HPP
