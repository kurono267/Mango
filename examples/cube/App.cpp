//
// Created by kurono267 on 2018-12-07.
//

#include "App.hpp"

bool App::init() {
    auto mainWnd = mainApp.lock();

    _instance = std::make_unique<vulkan::InstanceVK>();
    _instance->init("Cube", mainWnd->window(), mainWnd->wndSize());

    auto device = _instance->device();
    std::cout << device->device_name() << std::endl;

    RenderPattern rp;
    rp.viewport(Viewport(glm::vec2(0), mainWnd->wndSize()));
    rp.scissor(glm::ivec2(0), mainWnd->wndSize());

    _camera = std::make_shared<CameraAtPoint>(device,glm::vec3(0.0f));
    _camera->initProj(glm::radians(45.0f),(float)(1280)/(float)(720),0.1f,1000.0f);

    _texture = checkboardTexture(device, 1280, 720, 100);
    auto texView = _texture->createTextureView();

    _descSet = device->createDescSet();
    _descSet->setUniformBuffer(_camera->getCameraUniform(), 0, ShaderStage::Vertex);
    _descSet->setTexture(texView, Sampler(), 1, ShaderStage::Fragment);
    _descSet->create();

    _main = device->createPipeline(rp);
    _main->addShader(ShaderStage::Vertex, "../glsl/cube.vert");
    _main->addShader(ShaderStage::Fragment, "../glsl/cube.frag");
    _main->setDescSet(_descSet);

    _cube = createCube(device);

    spRenderPass renderPass = device->getScreenRenderPass();

    _main->setRenderPass(renderPass);
    _main->create();

    auto screenBuffers = device->getScreenbuffers();
    for (const auto &screen : screenBuffers) {
        std::cout << screen->info() << std::endl;
    }
    _cmdScreen.resize(screenBuffers.size());
    for (int i = 0; i < _cmdScreen.size(); ++i) {
        _cmdScreen[i] = device->createCommandBuffer();
        _cmdScreen[i]->begin();

        _cmdScreen[i]->setClearColor(0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        _cmdScreen[i]->setClearDepthStencil(1, 1.0f, 0.0f);

        _cmdScreen[i]->beginRenderPass(renderPass, screenBuffers[i],
                                       RenderArea(screenBuffers[i]->getSize(), glm::ivec2(0)));
        _cmdScreen[i]->bindPipeline(_main);
        _cmdScreen[i]->bindDescriptorSet(_main, _descSet);
        _cube->draw(_cmdScreen[i]);
        _cmdScreen[i]->endRenderPass();

        _cmdScreen[i]->end();
    }

    _screenAvailable = device->createSemaphore();
    _renderFinish = device->createSemaphore();

    return true;
}

bool App::draw() {
    auto currFrameTime = std::chrono::steady_clock::now();
    _dt = std::chrono::duration_cast<std::chrono::duration<float> >(currFrameTime-_prevFrameTime).count();

    auto device = _instance->device();
    auto imageIndex = device->nextScreen(_screenAvailable);

    device->submit(_cmdScreen[imageIndex],_screenAvailable,_renderFinish);
    device->present(imageIndex,_renderFinish);

    _prevFrameTime = currFrameTime;

    return true;
}

bool App::update() {
    _camera->updateUniform();
    return true;
}

bool App::onKey(const GLFWKey& key) {
    return true;
}
bool App::onMouse(const GLFWMouse& mouse) {
    _camera->onMouse(glm::vec2(mouse.x,mouse.y),_dt);
    return true;
}
bool App::onExit() {
    return true;
}
