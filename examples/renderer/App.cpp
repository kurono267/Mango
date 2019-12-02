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

	_cameraNode = std::make_shared<SceneNode>(std::make_shared<Camera>(glm::radians(45.0f),(float)(1280)/(float)(720),0.1f,1000.0f));
	_cameraNode->setPos(glm::vec3(0.f,0.f,-3.f));

	_cameraOrbit = std::make_shared<SceneNode>();
	_cameraOrbit->addChild(_cameraNode);

	//_scene.rootNode = std::make_shared<SceneNode>();

	_scene.rootNode = Assets::loadModel("bunny/scene.gltf");
	BBox sceneBox = _scene.rootNode->boundingBox();
	//std::cout << sceneBox << std::endl;
	auto center = (sceneBox.min+sceneBox.max)*0.5f;
	_scene.rootNode->setPos(-center);
	_scene.rootNode->addChild(_cameraOrbit);

	spSceneNode testScene = createTestScene();
	_scene.rootNode->addChild(testScene);

	_renderer = Renderer::make(device,glm::ivec2(1280,720));
	_renderer->init(_scene);

    return true;
}

spSceneNode App::createTestScene(){
	auto device = Instance::device();

	spSceneNode rootNode = std::make_shared<SceneNode>();

	spMaterial planeMaterial = std::make_shared<Material>(device);
	planeMaterial->setAlbedo(glm::vec4(1.f));
	planeMaterial->setMetallicFactor(0.1f);
	planeMaterial->setRoughnessFactor(0.01f);
	spGeometry plane = Geometry::make(createQuad(),planeMaterial);
	spSceneNode planeNode = std::make_shared<SceneNode>(plane);
	planeNode->setScale(glm::vec3(10.f,10.f,1.f));
	rootNode->addChild(planeNode);

	spMesh sphereMesh = createSphere(64);
	spGeometry sphere = Geometry::make(sphereMesh,planeMaterial);
	spSceneNode sphereNode = std::make_shared<SceneNode>(sphere);
	rootNode->addChild(sphereNode);

	return rootNode;
}

bool App::draw() {
    _renderer->render(_scene);

    return true;
}

bool App::update() {
    return true;
}

bool App::onTouch(const glm::vec2& coord, const glm::vec2& deltacoord) {
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

