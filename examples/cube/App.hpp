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
    ~App() final;

    bool init() final;
    bool draw() final;
    bool update() final;

    bool onTouchMove(const glm::vec2& coord, const glm::vec2& deltacoord) final;
    bool onExit() final;
protected:
    spPipeline _main;
    std::vector<spCommandBuffer> _cmdScreen;
    spMesh _cube;
    spTexture _texture;

    spSceneNode _cameraNode;
    spSceneNode _cameraOrbit;
    Uniform _cameraUniform;

    spDescSet _descSet;

    spSemaphore _screenAvailable;
    spSemaphore _renderFinish;
protected:
	void updateCameraUniform(const spSceneNode& cameraNode);
};


#endif //MANGO_APP_HPP
