//
// Created by kurono267 on 2019-06-26.
//

#ifndef MANGO_SCENETRANSFORM_HPP
#define MANGO_SCENETRANSFORM_HPP

#include "../api/default.hpp"

namespace mango {

class SceneTransform {
	public:
		SceneTransform();
		~SceneTransform();

		bool isUpdated();
		virtual void setUpdated(bool isUpdated);

		glm::mat4 transform() const;
		glm::vec3 pos() const;
		glm::quat rotationQuat() const;
		glm::vec3 rotationEuler() const;
		glm::vec3 scale() const;

		virtual void setTransform(const glm::mat4& transform);
		virtual void setPos(const glm::vec3& pos);
		virtual void setPos(float x, float y, float z);
		virtual void setRotation(const glm::quat& quat);
		virtual void setRotation(const glm::vec3& euler);
		virtual void setRotation(float eX,float eY,float eZ);
		virtual void setScale(const glm::vec3& scale);
		virtual void setScale(float sX,float sY,float sZ);
	protected:
		glm::mat4 _transform;
		glm::vec3 _pos;
		glm::quat _rotationQuat;
		glm::vec3 _rotationEuler;
		glm::vec3 _scale;

		bool _isUpdated;
	protected:
		glm::mat4 createTransform(const glm::vec3& pos, const glm::quat& rotation, const glm::vec3& scale);
};

}

#endif //MANGO_SCENETRANSFORM_HPP
