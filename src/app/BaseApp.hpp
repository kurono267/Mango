#pragma once

#include <memory>
#include <api/default.hpp>

namespace mango {

class MainApp;
struct GLFWKey;
struct GLFWMouse;
typedef std::shared_ptr<MainApp> spMainApp;

class BaseApp {
	public:
		BaseApp(spMainApp app) : mainApp(app){}
		virtual ~BaseApp(){}

		virtual bool init() = 0;
		virtual bool draw() = 0;
		virtual bool update() = 0;
		
		virtual bool onKey(const GLFWKey& key){
			return true;
		}
		virtual bool onMouse(const GLFWMouse& mouse){
			return true;
		}
		virtual bool onScroll(const glm::vec2& offset){
			return true;
		}
		virtual bool onExit() = 0;

		std::weak_ptr<MainApp> mainApp;
	protected: // Main access level
};
typedef std::shared_ptr<BaseApp> spBaseApp;

};
