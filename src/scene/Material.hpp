//
// Created by kurono267 on 2019-07-29.
//

#ifndef MANGO_MATERIAL_HPP
#define MANGO_MATERIAL_HPP

#include <api/Device.hpp>
#include <api/Texture.hpp>
#include <unified/Utils.hpp>

namespace mango {

class Material {
	public:
		Material(const spDevice& device);
		~Material() = default;

		void setAlbedo(const glm::vec4& value);
		void setAlbedo(const spTexture& texture);
		void setRoughness(const float& value);
		void setRoughness(const spTexture& texture);
		void setMetalness(const float& value);
		void setMetalness(const spTexture& texture);

		spTexture getAlbedo();
		spTexture getRoughness();
		spTexture getMetalness();

		spDescSet getDescSet();
		static spDescSet generalDescSet(const std::weak_ptr<Device> &device); // Desc set stored format of material
	protected:
		spTexture _albedo;
		spTexture _roughness;
		spTexture _metalness;

		spTextureView _albedoView;
		spTextureView _roughnessView;
		spTextureView _metalnessView;

		std::weak_ptr<Device> _device;
		spDescSet _descSet;
};

typedef std::shared_ptr<Material> spMaterial;

}

#endif //MANGO_MATERIAL_HPP
