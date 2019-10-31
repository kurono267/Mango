//
// Created by kurono267 on 2018-12-07.
//

#include "App.hpp"

bool App::init() {
    auto mainWnd = mainApp.lock();

    Instance::init<vulkan::InstanceVK>("Cube", mainWnd->window(), mainWnd->wndSize());

    auto device = Instance::device();
    std::cout << device->deviceName() << std::endl;

	_noise2d = createRandomTexture2D(512,512);

    _descSet = device->createDescSet();
    _descSet->setTexture(_noise2d->createTextureView(),Sampler(Filter::Nearest,Filter::Nearest),0,ShaderStage::Fragment);
    _descSet->create();

    PipelineInfo rp;
    rp.viewport(Viewport(glm::vec2(0), mainWnd->wndSize()));
    rp.scissor(glm::ivec2(0), mainWnd->wndSize());
    rp.addShader(ShaderStage::Vertex, "../glsl/procedure/main.vert");
    rp.addShader(ShaderStage::Fragment, "../glsl/procedure/main.frag");
    rp.setDescSet(_descSet);
    rp.setRenderPass(device->getScreenRenderPass());

    _main = device->createPipeline(rp);

    _quad = createQuad();

    auto screenBuffers = device->getScreenbuffers();
    _cmdScreen.resize(screenBuffers.size());
    for (int i = 0; i < _cmdScreen.size(); ++i) {
        _cmdScreen[i] = device->createCommandBuffer();
        _cmdScreen[i]->begin();

        _cmdScreen[i]->setClearColor(0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        _cmdScreen[i]->setClearDepthStencil(1, 1.0f, 0.0f);

        _cmdScreen[i]->beginRenderPass(rp.getRenderPass(), screenBuffers[i],
                                       RenderArea(screenBuffers[i]->getSize(), glm::ivec2(0)));
        _cmdScreen[i]->bindPipeline(_main);
        _cmdScreen[i]->bindDescriptorSet(_main,_descSet);
        _quad->draw(_cmdScreen[i]);
        _cmdScreen[i]->endRenderPass();

        _cmdScreen[i]->end();
    }

    _screenAvailable = device->createSemaphore();
    _renderFinish = device->createSemaphore();

    return true;
}

bool App::draw() {
    auto device = Instance::device();
    auto imageIndex = device->nextScreen(_screenAvailable);

    device->submit(_cmdScreen[imageIndex],_screenAvailable,_renderFinish);
    device->present(imageIndex,_renderFinish);

    return true;
}

bool App::update() {
    return true;
}

bool App::onTouch(const glm::vec2& coord, const glm::vec2& deltacoord){
	return true;
}

bool App::onExit() {
	Instance::device()->waitIdle();
    return true;
}

App::~App() {
	std::cout << "~App" << std::endl;
}

