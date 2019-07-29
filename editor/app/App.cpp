//
// Created by kurono267 on 2018-12-07.
//

#include "App.hpp"

struct Data {
	glm::mat4 view;
	glm::mat4 proj;
	glm::mat4 viewProj;
};

void App::updateCameraUniform(const spSceneNode& cameraNode) {
	Data data;
	data.view = cameraNode->getWorldTransform();
	data.proj = cameraNode->getCamera()->getProj();
	data.viewProj = data.proj*data.view;
	_cameraUniform.set(sizeof(Data),&data);
}

App::~App() {
	std::cout << "~App" << std::endl;
}

bool App::init() {
    auto mainWnd = mainApp.lock();

    _instance = std::make_unique<vulkan::InstanceVK>();
    _instance->init("Cube", mainWnd->window(), mainWnd->wndSize());

    auto device = _instance->device();
    std::cout << device->device_name() << std::endl;

    // Create scene with camera only
    _cameraNode = std::make_shared<SceneNode>(std::make_shared<Camera>(glm::radians(45.0f),(float)(1280)/(float)(720),0.1f,1000.0f));
	_cameraNode->setPos(glm::vec3(0.f,0.f,-5.f));

	_cameraOrbit = std::make_shared<SceneNode>();
	_cameraOrbit->addChild(_cameraNode);

    _cameraUniform.create(device,sizeof(Data));
    updateCameraUniform(_cameraNode);

	_texture = checkboardTexture(device, 1280, 720, 100);
	auto texView = _texture->createTextureView();

	_descSet = device->createDescSet();
	_descSet->setUniformBuffer(_cameraUniform, 0, ShaderStage::Vertex);
	_descSet->setTexture(texView, Sampler(), 1, ShaderStage::Fragment);
	_descSet->create();

    PipelineInfo rp;
    rp.viewport(Viewport(glm::vec2(0), mainWnd->wndSize()));
    rp.scissor(glm::ivec2(0), mainWnd->wndSize());
    rp.addShader(ShaderStage::Vertex, "../glsl/cube.vert");
    rp.addShader(ShaderStage::Fragment, "../glsl/cube.frag");
    rp.setDescSet(_descSet);
    rp.setRenderPass(device->getScreenRenderPass());

    _main = device->createPipeline(rp);

    _cube = createCube(device);

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

        _cmdScreen[i]->beginRenderPass(rp.getRenderPass(), screenBuffers[i],
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
    updateCameraUniform(_cameraNode);
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
            if(!_isFirst){
                glm::vec2 dp = glm::vec2(mouse.x,mouse.y)-_prev_mouse;
				glm::vec3 eulerAngles = _cameraOrbit->rotationEuler();
				eulerAngles.y += dp.x*_dt;
				eulerAngles.x += dp.y*_dt;
				_cameraOrbit->setRotation(eulerAngles);
            }
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

