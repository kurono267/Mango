//
// Created by kurono267 on 2018-12-07.
//

#ifndef MANGO_CAMERA_HPP
#define MANGO_CAMERA_HPP

#include <mango.hpp>

namespace mango::scene {

class Camera {
public:
    Camera(const spDevice& device){
        _uniform.create(device,sizeof(Data),&_data);
    }
    virtual ~Camera() = default;

    inline Uniform& getCameraUniform(){
        return _uniform;
    }

    inline void initProj(float angle,float aspect,float near,float far){
        _data.proj = glm::perspective(angle,aspect,near,far);
        rotate(glm::vec2(0.f),0.f);
    }

    inline void updateUniform(){
        _uniform.set(sizeof(Data),&_data);
    }

    virtual void rotate(const glm::vec2& deltaCoord, float dt) = 0;
    virtual void onKeyboard(uint8_t key) = 0;
protected:
    Uniform _uniform;
    struct Data {
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj;
    } _data;
};

class CameraAtPoint : public Camera {
public:
    CameraAtPoint(const spDevice& device,const glm::vec3& point);
    ~CameraAtPoint() final = default;

    void rotate(const glm::vec2 &deltaCoord, float dt) final;
    void onKeyboard(uint8_t key) final;
protected:
    glm::vec3 _pos;
    glm::vec3 _point;
    glm::vec3 _up;

    glm::vec2 _prevCoord;
    bool _isFirst = true;
};

}

#endif //MANGO_CAMERA_HPP
