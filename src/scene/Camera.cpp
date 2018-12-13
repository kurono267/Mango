//
// Created by kurono267 on 2018-12-07.
//

#include "Camera.hpp"

namespace mango::scene {

CameraAtPoint::CameraAtPoint(const spDevice &device, const glm::vec3 &point) : Camera(device),_point(point),_pos(0.0,0.0,-5.0f),_up(0.f,-1.f,0.f)
{}

void CameraAtPoint::rotate(const glm::vec2 &deltaCoord, float dt) {
    float angularSpeed = 0.1f*dt;
    float theta = deltaCoord.x * angularSpeed;
    float phi = deltaCoord.y * angularSpeed;

    auto viewVec = normalize(_point - _pos);
    float dist = length(_point - _pos);

    // Rotate along Y
    const auto dY = normalize(cross(viewVec,_up));
    glm::quat qY(phi,dY);

    //_up = normalize(qY*_up);

    const auto dX = normalize(qY*_up);

    glm::quat qX(theta,dX);
    glm::quat r = normalize(qX*qY);

    viewVec = normalize(r*viewVec);
    _pos = _point+dist*viewVec*glm::vec3(-1.0f,1.0f,-1.0f);

    _data.view = lookAt(_pos,_point,_up);
    _data.viewProj = _data.proj*_data.view;
}

void CameraAtPoint::onKeyboard(uint8_t key) {
    // Do nothing
}

}
