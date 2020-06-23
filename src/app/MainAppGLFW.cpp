#include "MainAppGLFW.hpp"
#include "../vk/InstanceVK.hpp"

using namespace mango;

MainAppGLFW::MainAppGLFW() : _isRun(true), _window(nullptr) {}

void MainAppGLFW::setBaseApp(spBaseApp app){
	_app = app;
}

void MainAppGLFW::exit(){
	_isRun = false;
}

bool MainAppGLFW::is(){
	return _isRun;
}

void* MainAppGLFW::window(){
	return _window;
}

void MainAppGLFW::resize(const int width,const int height){
	_app->onExit();
	_app->init();
}
			
void MainAppGLFW::run(){
	//_app->mainApp = shared_from_this();
	Instance::init<vulkan::InstanceVK>(_title, window(), frameSize());
	_app->init();
	while(_isRun){
		_app->update();
		_app->draw();
		glfwPollEvents();

		if(glfwWindowShouldClose(_window))_isRun = false;
	}
	_app->onExit();
	_app.reset();
	Instance::release();
}

glm::ivec2 MainAppGLFW::wndSize(){
	int width;
	int height;
	glfwGetWindowSize(_window,&width,&height);
	return glm::vec2(width,height);
}

glm::ivec2 MainAppGLFW::frameSize() {
	int width;
	int height;
	glfwGetFramebufferSize(_window,&width,&height);
	return glm::vec2(width,height);
}

void MainAppGLFW::create(const std::string& title,const int width,const int height){
	try {
		glfwSetErrorCallback(&MainAppGLFW::__glfwOnError);
		if (!glfwInit())
			::exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!_window){
			glfwTerminate();
			::exit(EXIT_FAILURE);
		}
		glfwSetKeyCallback(_window, &MainAppGLFW::__glfwOnKey);
		glfwSetCursorPosCallback(_window, &MainAppGLFW::__glfwOnMousePos);
		glfwSetMouseButtonCallback(_window, &MainAppGLFW::__glfwOnMouseBtn);
		glfwSetScrollCallback(_window, &MainAppGLFW::__glfwOnScroll);
		glfwSetWindowSizeCallback(_window, &MainAppGLFW::__glfwOnResize);

		_title = title;
	} catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

void MainAppGLFW::vsync(bool on){
	if(on)glfwSwapInterval(1);
	else glfwSwapInterval(0);
}

const KeyData MainAppGLFW::glfwKeyState() const {
	return _lastKey;
}
