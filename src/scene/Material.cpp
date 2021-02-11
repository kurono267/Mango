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
	_albedoColor = value;//setAlbedo(createSinglePixelTexture(value));
}

void Material::setMetallicRoughness(const spTexture& texture) {
	_metallicRoughness = texture;
}

uint32_t Material::getID() const {
	return _id;
}

void Material::setID(const uint32_t& id) {
	_id = id;
}

glm::vec4 Material::getAlbedo() {
	return _albedoColor;
}

void Material::setMetallic(const float metallic) {
	_metallic = metallic;
}

void Material::setRoughness(const float roughness) {
	_roughness = roughness;
}

float Material::getRoughness() const {
	return _roughness;
}

float Material::getMetallic() const {
	return _metallic;
}

spTexture Material::getAlbedoTexture() {
	return _albedo;
}

spTexture Material::getMetallicRoughnessTexture() {
	return _metallicRoughness;
}
