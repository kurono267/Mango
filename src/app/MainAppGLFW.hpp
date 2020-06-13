#pragma once

#include <functional>
#include <memory>
#include <iostream>

#include <GLFW/glfw3.h>
#include "BaseApp.hpp"
#include "MainApp.hpp"

namespace mango {

class MainAppGLFW : public MainApp,std::enable_shared_from_this<MainAppGLFW> {
	public:
		typedef std::shared_ptr<MainAppGLFW> ptr;

		MainAppGLFW();
		virtual ~MainAppGLFW(){
			std::cout << "MainApp Destructor" << std::endl;
		}

		void setBaseApp(spBaseApp app) override;

		const KeyData glfwKeyState() const override;

		void exit() override;
		// Create window and setup
		void create(const std::string& title,const int width,const int height);
		glm::ivec2 wndSize() override; // Return window size
		
		void vsync(bool on);

		void run() override;
		void resize(const int width,const int height);
		bool is() override;

		void* window() override;

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
			app->_lastKey = KeyData(key,scancode,action,mods);
			app->_app->onKey(app->glfwKeyState());
		}

		static void __glfwOnMouseBtn(GLFWwindow* window, int button, int action, int mods){
			ptr& app = instance();
			double x; double y;
			glfwGetCursorPos(app->_window,&x,&y);
			if(button == GLFW_MOUSE_BUTTON_LEFT) {
				if (action == GLFW_PRESS) {
					app->_prevMousePos = glm::vec2(x,y);
					app->_app->onTouchDown(glm::vec2(x, y));
				}
			}
		}

		static void __glfwOnMousePos(GLFWwindow* window, double x, double y){
			ptr& app = instance();
			if(glfwGetMouseButton(app->_window,GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
				double x; double y;
				glfwGetCursorPos(app->_window,&x,&y);
				glm::vec2 curr(x,y);
				app->_app->onTouch(curr,curr-app->_prevMousePos);
			}
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

		// Current statement
		glm::vec2 _prevMousePos;

		std::string _title;

		KeyData _lastKey;
		GLFWwindow* _window;

		bool _isRun;
};

};
