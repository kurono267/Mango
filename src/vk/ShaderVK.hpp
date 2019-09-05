//
// Created by kurono267 on 2019-09-05.
//

#ifndef MANGO_SHADERVK_HPP
#define MANGO_SHADERVK_HPP

#include "default.hpp"
#include <api/Types.hpp>
#include <optional>

namespace mango::vulkan {

class ShaderVK {
	public:
		static std::optional<vk::ShaderModule> createShader(const mango::ShaderStage &type, const std::string &filename);
};

}

#endif //MANGO_SHADERVK_HPP
