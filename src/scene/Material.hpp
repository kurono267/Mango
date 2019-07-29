//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_MATERIAL_HPP
#define MANGO_MATERIAL_HPP

#include <mango.hpp>

namespace mango {

class Material {
	public:
		Material() = default;
		~Material() = default;

		void setAlbedo(const spTexture& texture);
		void setAlbedo(const glm::vec4& color);

		void setRoughness(const spTexture& texture);
		void setRoughness(const float& value);

		void setMetalness(const spTexture& texture);
		void setMetalness(const float& value);
	protected:
		spTexture _albedo;
		spTexture _roughness;
		spTexture _metalness;
};

}

#endif //MANGO_MATERIAL_HPP
