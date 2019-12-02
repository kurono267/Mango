#include "MainApp.hpp"
#include "api/Instance.hpp"
#include "scene/Assets.hpp"

using namespace mango;

KeyData::KeyData() : key(0), scancode(0), action(0), mods(0) {}
KeyData::KeyData(int _key, int _scancode, int _action, int _mods) : key(_key), scancode(_scancode), action(_action), mods(_mods) {}

MainApp::MainApp() : _isRun(true), _window(nullptr),_isLeftBtn(false) {}

void MainApp::exit(){
	_isRun = false;
}

bool MainApp::is(){
	return _isRun;
}

GLFWwindow* MainApp::window(){
	return _window;
}

void MainApp::resize(const int width,const int height){
	_app->onExit();
	_app->init();
}
			
void MainApp::run(){
	_app->mainApp = shared_from_this();
	_app->init();
	while(_isRun){
		auto currFrameTime = std::chrono::steady_clock::now();
		_dt = std::chrono::duration_cast<std::chrono::duration<float> >(currFrameTime-_prevFrameTime).count();
		_prevFrameTime = currFrameTime;

		_app->update();
		_app->draw();
		glfwPollEvents();

		if(glfwWindowShouldClose(_window))_isRun = false;
	}
	_app->onExit();
	_app.reset();
	Assets::freeCache();
	Instance::release();
}

glm::ivec2 MainApp::wndSize(){
	int width;
	int height;
	glfwGetFramebufferSize(_window, &width, &height);
	return glm::vec2(width,height);
}

void MainApp::create(const std::string& title,const int width,const int height){
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

void MainApp::vsync(bool on){
	if(on)glfwSwapInterval(1);
	else glfwSwapInterval(0);
}

const KeyData MainApp::glfwKeyState() const {
	return _lastKey;
}

float MainApp::getFrameTime() {
	return _dt;
}
