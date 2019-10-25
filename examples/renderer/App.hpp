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

    bool onTouch(const glm::vec2& coord, const glm::vec2& deltacoord) final;

    bool onExit() final;
protected:
	spSceneNode createTestScene();

    spSceneNode _cameraNode;
    spSceneNode _cameraOrbit;

    Renderer::ptr _renderer;

    Scene _scene;
};


#endif //MANGO_APP_HPP
