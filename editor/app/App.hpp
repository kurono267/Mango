//
// Created by kurono267 on 2018-12-07.
//

#ifndef MANGO_EXAMPLE_APP_HPP
#define MANGO_EXAMPLE_APP_HPP

#define MANGO_VULKAN
#include <mango.hpp>
#include "scene/Camera.hpp"

using namespace mango;

class App : public BaseApp {
public:
    explicit App(const spMainApp& app) : BaseApp(app) {}
    virtual ~App() final;

    bool init() final;
    bool draw() final;
    bool update() final;

    bool onKey(const GLFWKey& key) final;
    bool onMouse(const GLFWMouse& mouse) final;
    bool onExit() final;
protected:
    std::unique_ptr<Instance> _instance;

    spPipeline _main;
    std::vector<spCommandBuffer> _cmdScreen;
    spMesh _cube;
    spTexture _texture;

    spSceneNode _cameraNode;
    spSceneNode _cameraOrbit;
    Uniform _cameraUniform;

    // For camera rotate
    bool _isPressed = false;
    bool _isFirst = false;
    glm::vec2 _prev_mouse;
    float _dt;
    std::chrono::steady_clock::time_point _prevFrameTime;

    spDescSet _descSet;

    spSemaphore _screenAvailable;
    spSemaphore _renderFinish;
protected:
	void updateCameraUniform(const spSceneNode& cameraNode);
};


#endif //MANGO_APP_HPP
