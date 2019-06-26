//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENETRANSFORM_HPP
#define MANGO_SCENETRANSFORM_HPP

#include <api/default.hpp>

namespace mango::scene {

class SceneTransform {
	public:
		SceneTransform();
		~SceneTransform();

		glm::mat4 transform() const;
		glm::vec3 pos() const;
		glm::quat rotationQuat() const;
		glm::vec3 rotationEuler() const;
		glm::vec3 scale() const;

		void setTransform(const glm::mat4& transform);
		void setPos(const glm::vec3& pos);
		void setRotation(const glm::quat& quat);
		void setRotation(const glm::vec3& euler);
		void setScale(const glm::vec3& scale);
	protected:
		glm::mat4 _transform;
		glm::vec3 _pos;
		glm::quat _rotationQuat;
		glm::vec3 _rotationEuler;
		glm::vec3 _scale;
	protected:
		glm::mat4 createTransform(const glm::vec3& pos, const glm::quat& rotation, const glm::vec3& scale);
};

}

#endif //MANGO_SCENETRANSFORM_HPP
