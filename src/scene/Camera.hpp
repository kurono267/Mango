//
// Created by kurono267 on 2018-12-07.
//

#ifndef MANGO_CAMERA_HPP
#define MANGO_CAMERA_HPP

#include <api/default.hpp>

namespace mango::scene {

class Camera {
	public:
		Camera(float angle,float aspect,float near,float far);
		virtual ~Camera() = default;

		glm::mat4 getProj();
	protected:
		glm::mat4 _proj;
};

typedef std::shared_ptr<Camera> spCamera;

}

#endif //MANGO_CAMERA_HPP
