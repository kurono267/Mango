//
// Created by kurono267 on 2019-07-29.
//

#include "Material.hpp"
#include <api/DescSet.hpp>

using namespace mango;

Material::Material(const spDevice &device) : _device(device),_descSet(nullptr) {
	_uniform.create(device,sizeof(_factors));
	_metallicRoughness = createSinglePixelTexture(device,glm::vec4(1,1,0,0));
}

void Material::setAlbedo(const spTexture &texture) {
	_albedo = texture;
}

void Material::setAlbedo(const glm::vec4 &value) {
	setAlbedo(createSinglePixelTexture(_device.lock(),value));
}

void Material::setRoughnessFactor(const float& value) {
	_roughnessFactor = value;
	_uniform.set(sizeof(_factors),&_factors);
}

void Material::setMetallicRoughness(const spTexture& texture) {
	_metallicRoughness = texture;
}

void Material::setMetallicFactor(const float& value) {
	_metallicFactor = value;
	_uniform.set(sizeof(_factors),&_factors);
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

float Material::getMetallicFactor() {
	return _metallicFactor;
}

float Material::getRoughnessFactor() {
	return _roughnessFactor;
}

spTexture Material::getMetallicRoughness() {
	return _metallicRoughness;
}

spDescSet Material::getDescSet() {
	if(!_descSet){
		auto deviceInstance = _device.lock();
		_descSet = deviceInstance->createDescSet();
		if(!_albedo)_albedo = createSinglePixelTexture(deviceInstance,glm::vec4(1.f));
		_albedoView = _albedo->createTextureView();
		_metallicRoughnessView = _metallicRoughness->createTextureView(ComponentMapping());
		_descSet->setTexture(_albedoView,Sampler(),0,ShaderStage::All); // TODO fix to support Fragment and compute shader stage
		_descSet->setTexture(_metallicRoughnessView,Sampler(),1,ShaderStage::All);
		_descSet->setUniformBuffer(_uniform,2,ShaderStage::All);
		_descSet->create();
	}
	return _descSet;
}

spDescSet Material::generalDescSet(const std::weak_ptr<Device> &device) {
	auto deviceInstance = device.lock();
	auto descSet = deviceInstance->createDescSet();
	spTexture albedo = createSinglePixelTexture(deviceInstance,glm::vec4(1.f));
	spTexture metallicRoughness = createSinglePixelTexture(deviceInstance,1.f);
	spTextureView albedoView = albedo->createTextureView();
	spTextureView mrView = metallicRoughness->createTextureView();
	Uniform uniform;
	uniform.create(deviceInstance,sizeof(glm::vec2));
	descSet->setTexture(albedoView,Sampler(),0,ShaderStage::All); // TODO fix to support Fragment and compute shader stage
	descSet->setTexture(mrView,Sampler(),1,ShaderStage::All);
	descSet->setUniformBuffer(uniform,2,ShaderStage::All);
	descSet->create();
	return descSet;
}
