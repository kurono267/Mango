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

bool App::init() {
    auto mainWnd = mainApp.lock();

    Instance::init<vulkan::InstanceVK>("CubeMap", mainWnd->window(), mainWnd->wndSize());

    auto device = Instance::device();
    Assets::init(device);
    std::cout << device->deviceName() << std::endl;

    auto frameSize = device->getScreenSize();

    // Create scene with camera only
    _cameraNode = std::make_shared<SceneNode>(std::make_shared<Camera>(glm::radians(45.0f),(float)(frameSize.x)/(float)(frameSize.y),0.1f,1000.0f));
	_cameraNode->setPos(glm::vec3(0.f,0.f,-5.f));

	_cameraOrbit = std::make_shared<SceneNode>();
	_cameraOrbit->addChild(_cameraNode);

    _cameraUniform.create(device,sizeof(Data));
    updateCameraUniform(_cameraNode);

    _ibl = std::make_shared<ImageBasedLight>(Assets::loadTexture("textures/IBL/fireplace_8k.hdr"),2048);

	_texture = _ibl->getCubeMap();
	auto texView = _ibl->getCubeMapView();

	_descSet = device->createDescSet();
	_descSet->setUniformBuffer(_cameraUniform, 0, ShaderStage::Vertex);
	_descSet->setTexture(texView, Sampler(), 1, ShaderStage::Fragment);
	_descSet->create();

	auto screenRTs = device->getScreenRenderTargets();

    PipelineInfo rp;
    rp.viewport(Viewport(glm::vec2(0), mainWnd->wndSize()));
    rp.scissor(glm::ivec2(0), mainWnd->wndSize());
    rp.addShader(ShaderStage::Vertex, "../glsl/cubemap.vert");
    rp.addShader(ShaderStage::Fragment, "../glsl/cubemap.frag");
    rp.rasterizer(PolygonMode::Fill,CullMode::Front);
    rp.setDescSet(_descSet);
    rp.setRenderPass(screenRTs[0]->renderPass());

    _main = device->createPipeline(rp);

    _cube = createCube();

    _cmdScreen.resize(screenRTs.size());
    for (int i = 0; i < _cmdScreen.size(); ++i) {
        _cmdScreen[i] = device->createCommandBuffer();
        _cmdScreen[i]->begin();

        _cmdScreen[i]->setClearColor(0, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
        _cmdScreen[i]->setClearDepthStencil(1, 1.0f, 0.0f);

        _cmdScreen[i]->beginRenderPass(rp.getRenderPass(), screenRTs[i]->framebuffer(),
                                       RenderArea(device->getScreenSize(), glm::ivec2(0)));
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
    auto device = Instance::device();
    auto imageIndex = device->nextScreen(_screenAvailable);

    device->submit(_cmdScreen[imageIndex],_screenAvailable,_renderFinish);
    device->present(imageIndex,_renderFinish);

    return true;
}

bool App::update() {
    updateCameraUniform(_cameraNode);
    return true;
}

bool App::onTouch(const glm::vec2& coord, const glm::vec2& deltacoord){
	glm::vec3 eulerAngles = _cameraOrbit->rotationEuler();
	eulerAngles.y += deltacoord.x;
	eulerAngles.x += deltacoord.y;
	_cameraOrbit->setRotation(eulerAngles);
	return true;
}

bool App::onExit() {
	Instance::device()->waitIdle();
    return true;
}

App::~App() {
	std::cout << "~App" << std::endl;
}

