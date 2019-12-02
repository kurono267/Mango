#pragma once

#include <functional>
#include <memory>
#include <iostream>

#include "BaseApp.hpp"

namespace mango {

struct KeyData {
	KeyData();
	KeyData(int _key, int _scancode, int _action, int _mods);
	KeyData(const KeyData& state) = default;
	virtual ~KeyData() = default;

	int key; int scancode; int action; int mods;
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

		const KeyData glfwKeyState() const;

		void exit();
		// Create window and setup
		void create(const std::string& title,const int width,const int height);
		glm::ivec2 wndSize(); // Return window size
		
		void vsync(bool on);

		void run();
		void resize(const int width,const int height);
		bool is();

		float getFrameTime();

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
			app->_lastKey = KeyData(key, scancode, action, mods);
			app->_app->onKey(app->glfwKeyState());
		}

		static void __glfwOnMouseBtn(GLFWwindow* window, int button, int action, int mods){
			ptr& app = instance();
			double x; double y;
			glfwGetCursorPos(app->window(),&x,&y);
			glm::vec2 currPoint(x,y);
			if(button == GLFW_MOUSE_BUTTON_1) {
				if(action == GLFW_PRESS) {
					app->_app->onTouchDown(currPoint);
					app->_isLeftBtn = true;
				} else {
					app->_isLeftBtn = false;
				}
			}
			app->_prevCoord = currPoint;
		}

		static void __glfwOnMousePos(GLFWwindow* window, double x, double y){
			ptr& app = instance();
			glm::vec2 currPoint(x,y);
			if(app->_isLeftBtn){
				auto deltaCoord = currPoint-app->_prevCoord;
				app->_app->onTouch(currPoint,deltaCoord*app->_dt);
			}
			app->_prevCoord = currPoint;
		}

		static void __glfwOnScroll(GLFWwindow* window, double x, double y){
			ptr& app = instance();
			glm::vec2 offset(x,y);
			app->_app->onScroll(offset);
		}

		static void __glfwOnResize(GLFWwindow* window, int width, int height){
			ptr& app = instance();
			std::cout << "resize" << std::endl;
			app->resize(width,height);
		}

		std::unique_ptr<BaseApp> _app;
		// Current statements
		KeyData   _lastKey;

		float _dt;
		std::chrono::steady_clock::time_point _prevFrameTime;
		glm::vec2 _prevCoord;
		bool _isLeftBtn;

		std::string _title;

		GLFWwindow* _window;

		bool _isRun;
};

};
