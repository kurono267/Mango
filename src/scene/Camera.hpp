//
// Created by kurono267 on 2018-12-07.
//

#ifndef MANGO_CAMERA_HPP
#define MANGO_CAMERA_HPP

#include "../api/default.hpp"

namespace mango {
/// Simple perspective camera
class Camera {
	public:
		/// Construct camera with perspective projection matrix
		/// @param angle - fov angle in radians
		/// @param aspect - aspect ratio
		/// @param near - near plane
		/// @param far - far plane
		Camera(float angle,float aspect,float near,float far);
		virtual ~Camera() = default;

		/// Get projection matrix
		glm::mat4 getProj();
	protected:
		glm::mat4 _proj;
};

typedef std::shared_ptr<Camera> spCamera;

}

#endif //MANGO_CAMERA_HPP
