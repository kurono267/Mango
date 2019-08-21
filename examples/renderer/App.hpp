//
// Created by kurono267 on 2018-12-07.
//

#ifndef MANGO_EXAMPLE_APP_HPP
#define MANGO_EXAMPLE_APP_HPP

#define MANGO_VULKAN
#include <mango.hpp>
#include <renderer/Renderer.hpp>
#include "scene/Camera.hpp"

using namespace mango;

class App : public BaseApp {
public:
    explicit App(const spMainApp& app) : BaseApp(app) {}
    ~App() final;

    bool init() final;
    bool draw() final;
    bool update() final;

    bool onKey(const GLFWKey& key) final;
    bool onMouse(const GLFWMouse& mouse) final;
    bool onExit() final;
protected:
    std::unique_ptr<Instance> _instance;

    spSceneNode _cameraNode;
    spSceneNode _cameraOrbit;

    Renderer::ptr _renderer;

    // For camera rotate
    bool _isPressed = false;
    bool _isFirst = false;
    glm::vec2 _prev_mouse;
    float _dt;
    std::chrono::steady_clock::time_point _prevFrameTime;
};


#endif //MANGO_APP_HPP
