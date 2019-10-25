#pragma once

#include <functional>
#include <memory>
#include <iostream>

#include "BaseApp.hpp"

namespace mango {

struct GLFWKey {
	GLFWKey();
	GLFWKey(int _key,int _scancode,int _action,int _mods);
	GLFWKey(const GLFWKey& state) = default;
	virtual ~GLFWKey() = default;

	int key; int scancode; int action; int mods;
};

struct GLFWMouse {
	GLFWMouse();
	GLFWMouse(const int _button,const int _action,const int _mods);
	GLFWMouse(const double _x,const double _y);
	GLFWMouse(const GLFWMouse& state) = default;
	virtual ~GLFWMouse() = default;

	static const int noState = 0x0000;
	static const int onMouseButton = 0x0001;
	static const int onMousePosition = 0x0010;

	int callState;
	int button; int action; int mods; // Mouse BTN
	double x; double y; // Mouse Position
};

//class BaseApp;

class MainApp : public std::enable_shared_from_this<MainApp> {
	public:
		typedef std::shared_ptr<MainApp> ptr;

		MainApp();
		virtual ~MainApp(){
			std::cout << "MainApp Destructor" << std::endl;
		}

		template<typename T>
		void createApplication() {
			_app = std::make_unique<T>(shared_from_this());
		}

		const GLFWKey glfwKeyState() const;
		const GLFWMouse glfwMouseState() const;

		void exit();
		// Create window and setup
		void create(const std::string& title,const int width,const int height);
		glm::ivec2 wndSize(); // Return window size
		
		void vsync(bool on);

		void run();
		void resize(const int width,const int height);
		bool is();

		GLFWwindow* window();

		static ptr& instance(){
			static ptr app; // lazy singleton, instantiated on first use
			if(!app)app = std::make_shared<MainApp>();
			return app;
		}
	protected:
		static void __glfwOnError(int error, const char* description){
			//throw app_exception(description);
			std::cout << description << std::endl;
		}

		static void __glfwOnKey(GLFWwindow* window, int key, int scancode, int action, int mods){
			ptr& app = instance();
			app->_lastKey = GLFWKey(key,scancode,action,mods);
			app->_app->onKey(app->glfwKeyState());
		}

		static void __glfwOnMouseBtn(GLFWwindow* window, int button, int action, int mods){
			ptr& app = instance();
			app->_lastMouse = GLFWMouse(button,action, mods);
			app->_app->onMouse(app->glfwMouseState());
		}

		static void __glfwOnMousePos(GLFWwindow* window, double x, double y){
			ptr& app = instance();
			app->_lastMouse = GLFWMouse(x,y);
			app->_app->onMouse(app->glfwMouseState());
		}

		static void __glfwOnScroll(GLFWwindow* window, double x, double y){
			ptr& app = instance();
			glm::vec2 offset(x,y);
			app->_app->onScroll(offset);
		}

		static void __glfwOnResize(GLFWwindow* window, int width, int height){
			ptr& app = instance();
			app->resize(width,height);
		}

		std::unique_ptr<BaseApp> _app;
		// Current statements
		GLFWKey   _lastKey;
		GLFWMouse _lastMouse;

		std::string _title;

		GLFWwindow* _window;

		bool _isRun;
};

};
