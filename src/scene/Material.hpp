//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_MATERIAL_HPP
#define MANGO_MATERIAL_HPP

#include "../api/Device.hpp"
#include "../api/Texture.hpp"
#include "../unified/Utils.hpp"
#include "../unified/Uniform.hpp"

namespace mango {

class Material {
	public:
		Material();
		~Material() = default;

		static std::shared_ptr<Material> create();

		void setAlbedo(const glm::vec4& value);
		void setAlbedo(const spTexture& texture);
		void setMetallic(const float metallic);
		void setRoughness(const float roughness);
		void setMetallicRoughness(const spTexture& texture);

		uint32_t getID();
		void setID(const uint32_t& id);

		spTexture getAlbedo();
		spTexture getMetallicRoughness();
		float getRoughness();
		float getMetallic();
	protected:
		spTexture _albedo;
		float _roughness = 1.f;
		float _metallic = 1.f;
		spTexture _metallicRoughness;

		uint32_t _id;
		spDevice _device;
};

typedef std::shared_ptr<Material> spMaterial;

}

#endif //MANGO_MATERIAL_HPP
