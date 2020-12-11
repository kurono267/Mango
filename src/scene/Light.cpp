//
// Created by kurono267 on 11.12.2020.
//

#include "Light.hpp"

using namespace mango;


Light::Light(const LightType &_type, const glm::vec3 &_color, bool _isCastShadow, float _radius) :
	type(_type), color(_color), isCastShadow(_isCastShadow), radius(_radius) {

}

std::shared_ptr<Light> Light::createPointLight(const glm::vec3 &color, bool isCastShadow, float radius) {
	return std::make_shared<Light>(PointLight,color,isCastShadow,radius);
}

std::shared_ptr<Light> Light::createDirLight(const glm::vec3 &color, bool isCastShadow) {
	return std::make_shared<Light>(DirLight,color,isCastShadow,-1.f);
}

std::shared_ptr<Light> Light::createSkyLight(bool isCastShadow) {
	return std::make_shared<Light>(SkyLight,glm::vec3(1),isCastShadow,-1.f);
}
