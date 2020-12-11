//
// Created by kurono267 on 11.12.2020.
//

#pragma once

#include <api/default.hpp>
#include <memory>

namespace mango {

enum LightType {
	PointLight,
	DirLight,
	SkyLight
};

class Light {
	public:
		Light(const LightType& type, const glm::vec3& color, bool isCastShadow, float radius);

		static std::shared_ptr<Light> createPointLight(const glm::vec3& color, bool isCastShadow, float radius);
		static std::shared_ptr<Light> createDirLight(const glm::vec3& color, bool isCastShadow);
		static std::shared_ptr<Light> createSkyLight(bool isCastShadow);

		LightType type;
		glm::vec3 color;
		float radius;
		bool 	  isCastShadow;
};

typedef std::shared_ptr<Light> spLight;

}

