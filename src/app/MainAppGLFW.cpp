#include "MainAppGLFW.hpp"
#include <GLFW/glfw3.h>

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
			
void MainAppGLFW::run(){
	//_app->mainApp = shared_from_this();
	_app->init();
	while(_isRun){
		_app->update();
		_app->draw();
		glfwPollEvents();

		if(glfwWindowShouldClose((GLFWwindow*)_window))_isRun = false;
	}
	_app->onExit();
	_app.reset();
}

glm::ivec2 MainAppGLFW::wndSize(){
	int width;
	int height;
	glfwGetWindowSize((GLFWwindow*)_window,&width,&height);
	return glm::vec2(width,height);
}

glm::ivec2 MainAppGLFW::frameSize() {
	int width;
	int height;
	glfwGetFramebufferSize((GLFWwindow*)_window,&width,&height);
	return glm::vec2(width,height);
}

void MainAppGLFW::onKey(int key, int scancode, int action, int mods){
	_lastKey = KeyData(key,scancode,action,mods);
	_app->onKey(keyState());
}

void MainAppGLFW::onMouseBtn(int button, int action, int mods){
	double x; double y;
	glfwGetCursorPos((GLFWwindow*)_window,&x,&y);
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		if (action == GLFW_PRESS) {
			_prevMousePos = glm::vec2(x,y);
			_prevMouseTime = std::chrono::system_clock::now();
			_app->onTouchDown(glm::vec2(x, y));
		}
	}
}

void MainAppGLFW::onMousePos(double x, double y){
	if(glfwGetMouseButton((GLFWwindow*)_window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
		glm::vec2 curr(x,y);
		glm::vec2 delta = curr-_prevMousePos;
		_prevMousePos = curr;
		auto now = std::chrono::system_clock::now();
		float dt = std::chrono::duration<float>(now-_prevMouseTime).count();
		_app->onTouch(curr,delta*dt);
		_prevMouseTime = now;
	}
}

void MainAppGLFW::onScroll(double x, double y){
	_app->onScroll(glm::vec2(x,y));
}

void MainAppGLFW::onResize(int width, int height){
	_app->onResize(width,height);
}

void MainAppGLFW::create(const std::string& title,const int width,const int height){
	try {
		glfwSetErrorCallback([](int error, const char* description){
			std::cout << description << std::endl;
		});
		if (!glfwInit())
			::exit(EXIT_FAILURE);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		_window = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);
		if (!_window){
			glfwTerminate();
			::exit(EXIT_FAILURE);
		}
		glfwSetWindowUserPointer((GLFWwindow*)_window, this);
		glfwSetKeyCallback((GLFWwindow*)_window, [](GLFWwindow* window, int key, int scancode, int action, int mods){
			auto app = (MainAppGLFW*)glfwGetWindowUserPointer(window);
			app->onKey(key,scancode,action,mods);
		});
		glfwSetCursorPosCallback((GLFWwindow*)_window, [](GLFWwindow* window, double x, double y){
			auto app = (MainAppGLFW*)glfwGetWindowUserPointer(window);
			app->onMousePos(x,y);
		});
		glfwSetMouseButtonCallback((GLFWwindow*)_window, [](GLFWwindow* window,int button, int action, int mods){
			auto app = (MainAppGLFW*)glfwGetWindowUserPointer(window);
			app->onMouseBtn(button,action,mods);
		});
		glfwSetScrollCallback((GLFWwindow*)_window, [](GLFWwindow* window,double x, double y){
			auto app = (MainAppGLFW*)glfwGetWindowUserPointer(window);
			app->onScroll(x,y);
		});
		glfwSetWindowSizeCallback((GLFWwindow*)_window, [](GLFWwindow* window,int width,int height){
			auto app = (MainAppGLFW*)glfwGetWindowUserPointer(window);
			app->onResize(width,height);
		});

		_title = title;
	} catch(std::exception& e){
		std::cout << e.what() << std::endl;
	}
}

void MainAppGLFW::vsync(bool on){
	if(on)glfwSwapInterval(1);
	else glfwSwapInterval(0);
}

const KeyData MainAppGLFW::keyState() const {
	return _lastKey;
}
