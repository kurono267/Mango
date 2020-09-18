//
// Created by kurono267 on 2019-07-29.
//

#include <api/Instance.hpp>
#include "Material.hpp"
#include "../api/DescSet.hpp"

using namespace mango;

Material::Material() {

}

std::shared_ptr<Material> Material::create() {
	return std::make_shared<Material>();
}

void Material::setAlbedo(const spTexture &texture) {
	_albedo = texture;
}

void Material::setAlbedo(const glm::vec4 &value) {
	setAlbedo(createSinglePixelTexture(value));
}

void Material::setMetallicRoughness(const float metallic,const float roughness) {
	setMetallicRoughness(createSinglePixelTexture(glm::vec2(metallic,roughness)));
}

void Material::setMetallicRoughness(const spTexture& texture) {
	_metallicRoughness = texture;
}

uint32_t Material::getID() {
	return _id;
}

void Material::setID(const uint32_t& id) {
	_id = id;
}

spTexture Material::getAlbedo() {
	return _albedo;
}

spTexture Material::getMetallicRoughness() {
	return _metallicRoughness;
}
