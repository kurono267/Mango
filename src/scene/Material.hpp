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
		void setMetallic(float metallic);
		void setRoughness(float roughness);
		void setMetallicRoughness(const spTexture& texture);

		uint32_t getID() const;
		void setID(const uint32_t& id);

		spTexture getAlbedoTexture();
		spTexture getMetallicRoughnessTexture();
		glm::vec4 getAlbedo();
		float getRoughness() const;
		float getMetallic() const;
	protected:
		spTexture _albedo;
		glm::vec4 _albedoColor = glm::vec4(1);
		float _roughness = 1.f;
		float _metallic = 1.f;
		spTexture _metallicRoughness;

		uint32_t _id;
		spDevice _device;
};

typedef std::shared_ptr<Material> spMaterial;

}

#endif //MANGO_MATERIAL_HPP
