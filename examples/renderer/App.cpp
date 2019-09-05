//
// Created by kurono267 on 2018-12-07.
//

#include <scene/Assets.hpp>
#include "App.hpp"

bool App::init() {
    auto mainWnd = mainApp.lock();

    Instance::init<vulkan::InstanceVK>("Renderer", mainWnd->window(), mainWnd->wndSize());

	auto device = Instance::device();
	Assets::init(device);
	std::cout << device->deviceName() << std::endl;

	_cameraNode = std::make_shared<SceneNode>(std::make_shared<Camera>(glm::radians(45.0f),(float)(1280)/(float)(720),1.f,10000.0f));
	_cameraNode->setPos(glm::vec3(0.f,0.f,-1000.f));

	_cameraOrbit = std::make_shared<SceneNode>();
	_cameraOrbit->addChild(_cameraNode);

	_scene.rootNode = Assets::loadModel("littlest_tokyo/scene.gltf");
	BBox sceneBox = _scene.rootNode->boundingBox();
	std::cout << sceneBox << std::endl;
	auto center = (sceneBox.min+sceneBox.max)*0.5f;
	//_scene.rootNode->setPos(-center);
	_scene.rootNode->addChild(_cameraOrbit);

	auto screenBuffers = device->getScreenbuffers();
	for (const auto &screen : screenBuffers) {
		std::cout << screen->info() << std::endl;
	}

	// Test BVH
	_scene.rootNode->run([](const spSceneNode& node, bool& isStop){
		auto geometry = node->getGeometry();
		if(!geometry)return;
		auto mesh = geometry->getMesh();
		if(!mesh)return;
		spBVH bvh = std::make_shared<BVH>(mesh);
		for(auto& bvhNode : bvh->nodes()){
			std::cout << bvhNode;
		}
	});

	_renderer = Renderer::make(device,glm::ivec2(1280,720));
	_renderer->init(_scene);

    return true;
}

bool App::draw() {
    auto currFrameTime = std::chrono::steady_clock::now();
    _dt = std::chrono::duration_cast<std::chrono::duration<float> >(currFrameTime-_prevFrameTime).count();

    _renderer->render(_scene);

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
	Instance::device()->waitIdle();
    return true;
}

App::~App() {
	std::cout << "~App" << std::endl;
}

