//
// Created by kurono267 on 2019-06-26.
//

#include "SceneTransform.hpp"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

using namespace mango;

SceneTransform::SceneTransform() {
	_scale = glm::vec3(1.f);
	_pos = glm::vec3(0.f,0.f,0.f);
	setRotation(glm::vec3(0.f));
}

SceneTransform::~SceneTransform() {

}

glm::mat4 SceneTransform::transform() const {
	return _transform;
}

glm::vec3 SceneTransform::pos() const {
	return _pos;
}

glm::quat SceneTransform::rotationQuat() const {
	return _rotationQuat;
}

glm::vec3 SceneTransform::rotationEuler() const {
	return _rotationEuler;
}

glm::vec3 SceneTransform::scale() const {
	return _scale;
}

void SceneTransform::setTransform(const glm::mat4& transform) {
	_transform = transform;
	// Update pos, rotation and scale
	glm::vec3 skew; glm::vec4 proj;
	glm::decompose(_transform,_scale,_rotationQuat,_pos,skew,proj);
	_rotationEuler = glm::eulerAngles(_rotationQuat);
}

void SceneTransform::setPos(const glm::vec3& pos) {
	_pos = pos;
	// Update transform
	_transform = createTransform(_pos,_rotationQuat,_scale);
}

void SceneTransform::setRotation(const glm::quat& quat) {
	_rotationQuat = quat;
	// Update rotationEuler and transform
	_rotationEuler = glm::eulerAngles(quat);
	_transform = createTransform(_pos,_rotationQuat,_scale);
}

void SceneTransform::setRotation(const glm::vec3& euler) {
	_rotationEuler = euler;
	// Update rotationQuat and transform
	_rotationQuat = glm::toQuat( glm::orientate3( euler ) );
	_transform = createTransform(_pos,_rotationQuat,_scale);
}

void SceneTransform::setScale(const glm::vec3& scale) {
	_scale = scale;
	// Update transform
	_transform = createTransform(_pos,_rotationQuat,_scale);
}

glm::mat4 SceneTransform::createTransform(const glm::vec3& pos, const glm::quat& rotation, const glm::vec3& scale) {
	glm::mat4 rotMat = glm::mat4_cast(rotation);
	glm::mat4 scaleMat = glm::scale(scale);
	glm::mat4 translateMat = glm::translate(pos);
	return scaleMat*rotMat*translateMat;
}
