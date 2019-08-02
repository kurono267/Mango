//
// Created by kurono267 on 2019-07-29.
//

#include "Material.hpp"

using namespace mango;

Material::Material(const spDevice &device) : _device(device),_descSet(nullptr) {

}

void Material::setAlbedo(const spTexture &texture) {
	_albedo = texture;
}

void Material::setAlbedo(const glm::vec4 &value) {
	setAlbedo(createSinglePixelTexture(_device.lock(),value));
}

void Material::setRoughness(const spTexture &texture) {
	_roughness = texture;
}

void Material::setRoughness(const float &value) {
	setRoughness(createSinglePixelTexture(_device.lock(),value));
}

void Material::setMetalness(const spTexture &texture) {
	_metalness = texture;
}

void Material::setMetalness(const float &value) {
	setMetalness(createSinglePixelTexture(_device.lock(),value));
}

spTexture Material::getAlbedo() {
	return _albedo;
}

spTexture Material::getRoughness() {
	return _roughness;
}

spTexture Material::getMetalness() {
	return _metalness;
}

spDescSet Material::getDescSet() {
	if(!_descSet){
		auto deviceInstance = _device.lock();
		_descSet = deviceInstance->createDescSet();
		_albedoView = _albedo->createTextureView();
		_roughnessView = _roughness->createTextureView(ComponentMapping(ComponentSwizzle::R,ComponentSwizzle::G,ComponentSwizzle::B,ComponentSwizzle::One));
		_metalnessView = _metalness->createTextureView(ComponentMapping(ComponentSwizzle::R,ComponentSwizzle::G,ComponentSwizzle::B,ComponentSwizzle::One));
		_descSet->setTexture(_albedoView,Sampler(),0,ShaderStage::Fragment);
		_descSet->setTexture(_roughnessView,Sampler(),1,ShaderStage::Fragment);
		_descSet->setTexture(_metalnessView,Sampler(),2,ShaderStage::Fragment);
		_descSet->create();
	}
	return _descSet;
}
