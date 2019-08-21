//
// Created by kurono267 on 2018-12-07.
//

#include <scene/Assets.hpp>
#include "App.hpp"

bool App::init() {
    auto mainWnd = mainApp.lock();

    _instance = std::make_unique<vulkan::InstanceVK>();
    _instance->init("Renderer", mainWnd->window(), mainWnd->wndSize());

    auto device = _instance->device();
    Assets::init(device);
    std::cout << device->device_name() << std::endl;

	_renderer = Renderer::make(device,glm::ivec2(1280,720));

    return true;
}

bool App::draw() {
    auto currFrameTime = std::chrono::steady_clock::now();
    _dt = std::chrono::duration_cast<std::chrono::duration<float> >(currFrameTime-_prevFrameTime).count();

    _renderer->render();

    _prevFrameTime = currFrameTime;

    return true;
}

bool App::update() {
    return true;
}

bool App::onKey(const GLFWKey& key) {
    return true;
}
bool App::onMouse(const GLFWMouse& mouse) {
    if(mouse.callState == GLFWMouse::onMouseButton){
        if(mouse.button == GLFW_MOUSE_BUTTON_1){
            if(mouse.action == GLFW_PRESS){
                _isPressed = true;
                _isFirst = true;
            } else _isPressed = false;
        }
    } else if (mouse.callState == GLFWMouse::onMousePosition){
        if(_isPressed){
            /*if(!_isFirst){
                glm::vec2 dp = glm::vec2(mouse.x,mouse.y)-_prev_mouse;
				glm::vec3 eulerAngles = _cameraOrbit->rotationEuler();
				eulerAngles.y += dp.x*_dt;
				eulerAngles.x += dp.y*_dt;
				_cameraOrbit->setRotation(eulerAngles);
            }*/
            _prev_mouse = glm::vec2(mouse.x,mouse.y);
            _isFirst = false;
        }
    }
    return true;
}
bool App::onExit() {
	_instance->device()->waitIdle();
    return true;
}

App::~App() {
	std::cout << "~App" << std::endl;
}

