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
    virtual void scale(const float& dvalue,const float& dt) = 0;
    virtual void onKeyboard(uint8_t key) = 0;

    inline glm::mat4 getView() {
        return _data.view;
    }

    inline glm::mat4 getProj() {
        return _data.proj;
    }

    virtual glm::vec3 getForward() = 0;
    virtual glm::vec3 getUp() = 0;
    virtual glm::vec3 getRight() = 0;
    virtual glm::vec3 getPos() = 0;

    bool isUpdated() { return _updated; }
    void updateFinish() { _updated = false; }
protected:
    Uniform _uniform;
    struct Data {
        glm::mat4 view;
        glm::mat4 proj;
        glm::mat4 viewProj;
    } _data;
    bool _updated;
};

class CameraAtPoint : public Camera {
public:
    CameraAtPoint(const spDevice& device,const glm::vec3& pos = glm::vec3(0.0,0.0,-5.0f),const glm::vec3& point = glm::vec3(0.0f));
    ~CameraAtPoint() final = default;

    void rotate(const glm::vec2 &deltaCoord, float dt) final;
    void onKeyboard(uint8_t key) final;
    void scale(const float& dvalue,const float& dt) final;

    glm::vec3 getForward() override;
    glm::vec3 getUp() override;
    glm::vec3 getRight() override;
    glm::vec3 getPos() override;
protected:
    glm::vec3 _pos;
    glm::vec3 _point;
    glm::vec3 _up;
    glm::vec3 _right;

    glm::vec2 _prevCoord;
    bool _isFirst = true;
};

typedef std::shared_ptr<Camera> spCamera;

}

#endif //MANGO_CAMERA_HPP
