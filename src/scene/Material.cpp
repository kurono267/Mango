//
// Created by kurono267 on 2019-07-29.
//

#include "Material.hpp"
#include <api/DescSet.hpp>

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

uint32_t Material::getID() {
	return _id;
}

void Material::setID(const uint32_t& id) {
	_id = id;
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
		_roughnessView = _roughness->createTextureView(ComponentMapping());
		_metalnessView = _metalness->createTextureView(ComponentMapping());
		_descSet->setTexture(_albedoView,Sampler(),0,ShaderStage::Fragment);
		_descSet->setTexture(_roughnessView,Sampler(),1,ShaderStage::Fragment);
		_descSet->setTexture(_metalnessView,Sampler(),2,ShaderStage::Fragment);
		_descSet->create();
	}
	return _descSet;
}

spDescSet Material::generalDescSet(const std::weak_ptr<Device> &device) {
	auto deviceInstance = device.lock();
	auto descSet = deviceInstance->createDescSet();
	spTexture albedo = createSinglePixelTexture(deviceInstance,glm::vec4(1.f));
	spTexture rm = createSinglePixelTexture(deviceInstance,1.f);
	spTextureView albedoView = albedo->createTextureView();
	spTextureView roughnessView = rm->createTextureView(ComponentMapping());
	spTextureView metalView = rm->createTextureView(ComponentMapping());
	descSet->setTexture(albedoView,Sampler(),0,ShaderStage::Fragment);
	descSet->setTexture(roughnessView,Sampler(),1,ShaderStage::Fragment);
	descSet->setTexture(metalView,Sampler(),2,ShaderStage::Fragment);
	descSet->create();
	return descSet;
}
