//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_MATERIAL_HPP
#define MANGO_MATERIAL_HPP

#include <api/Device.hpp>
#include <api/Texture.hpp>
#include <unified/Utils.hpp>
#include <unified/Uniform.hpp>

namespace mango {

class Material {
	public:
		Material(const spDevice& device);
		~Material() = default;

		void setAlbedo(const glm::vec4& value);
		void setAlbedo(const spTexture& texture);
		void setRoughnessFactor(const float& value);
		void setMetallicRoughness(const spTexture& texture);
		void setMetallicFactor(const float& value);

		uint32_t getID();
		void setID(const uint32_t& id);

		spTexture getAlbedo();
		float getMetallicFactor();
		float getRoughnessFactor();
		spTexture getMetallicRoughness();

		spDescSet getDescSet();
		static spDescSet generalDescSet(const std::weak_ptr<Device> &device); // Desc set stored format of material
	protected:
		spTexture _albedo;
		union {
			struct {
				float _roughnessFactor;
				float _metallicFactor;
			};
			glm::vec2 _factors;
		};
		spTexture _metallicRoughness;

		spTextureView _albedoView;
		spTextureView _metallicRoughnessView;

		std::weak_ptr<Device> _device;
		spDescSet _descSet;

		Uniform _uniform;

		uint32_t _id;
};

typedef std::shared_ptr<Material> spMaterial;

}

#endif //MANGO_MATERIAL_HPP
