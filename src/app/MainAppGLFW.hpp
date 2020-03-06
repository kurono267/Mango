#pragma once

#include <functional>
#include <memory>
#include <iostream>

#include "BaseApp.hpp"

namespace mango {

class MainAppGLFW : public MainApp,std::enable_shared_from_this<MainAppGLFW> {
	public:
		typedef std::shared_ptr<MainAppGLFW> ptr;

		MainAppGLFW();
		virtual ~MainAppGLFW(){
			std::cout << "MainApp Destructor" << std::endl;
		}

		void setBaseApp(spBaseApp app);

		const GLFWKey keyState() const;
		const GLFWMouse mouseState() const;

		void exit();
		// Create window and setup
		void create(const std::string& title,const int width,const int height);
		glm::ivec2 wndSize(); // Return window size
		
		void vsync(bool on);

		void run();
		void resize(const int width,const int height);
		bool is();

		void* window();

		static ptr& instance(){
			static ptr app; // lazy singleton, instantiated on first use
			if(!app)app = std::make_shared<MainAppGLFW>();
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

		spBaseApp _app;
		// Current statements
		GLFWKey   _lastKey;
		GLFWMouse _lastMouse;

		std::string _title;

		GLFWwindow* _window;

		bool _isRun;
};

};
