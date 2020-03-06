#include "MainAppGLFW.hpp"

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
	_app->mainApp = shared_from_this();
	_app->init();
	while(_isRun){
		_app->update();
		_app->draw();
		glfwPollEvents();

		if(glfwWindowShouldClose(_window))_isRun = false;
	}
	_app->onExit();
}

glm::ivec2 MainAppGLFW::wndSize(){
	int width;
	int height;
	glfwGetWindowSize(_window, &width, &height);
	return glm::vec2(width,height);
}

void MainAppGLFW::create(const std::string& title,const int width,const int height){
	try {
		glfwSetErrorCallback(&MainApp::__glfwOnError);
		if (!glfwInit())
			::exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!_window){
			glfwTerminate();
			::exit(EXIT_FAILURE);
		}
		glfwSetKeyCallback(_window, &MainApp::__glfwOnKey);
		glfwSetCursorPosCallback(_window, &MainApp::__glfwOnMousePos);
		glfwSetMouseButtonCallback(_window, &MainApp::__glfwOnMouseBtn);
		glfwSetScrollCallback(_window, &MainApp::__glfwOnScroll);
		glfwSetWindowSizeCallback(_window, &MainApp::__glfwOnResize);

		_title = title;
	} catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

void MainAppGLFW::vsync(bool on){
	if(on)glfwSwapInterval(1);
	else glfwSwapInterval(0);
}

const GLFWKey MainAppGLFW::keyState() const {
	return _lastKey;
}

const GLFWMouse MainAppGLFW::mouseState() const {
	return _lastMouse;
}
