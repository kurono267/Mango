//
// Created by kurono267 on 2018-12-07.
//

#include "Camera.hpp"

namespace mango::scene {

	Camera::Camera(float angle,float aspect,float near,float far){
		_proj = glm::perspective(angle,aspect,near,far);
	}

	glm::mat4 Camera::getProj() {
		return _proj;
	}

}
