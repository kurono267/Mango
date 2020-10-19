#pragma once

#include <memory>
#include "../api/default.hpp"

namespace mango {

class MainApp;
struct KeyData;
struct GLFWMouse;
typedef std::shared_ptr<MainApp> spMainApp;

class BaseApp {
	public:
		explicit BaseApp(const spMainApp& app) : mainApp(app){}
		virtual ~BaseApp() = default;

		virtual bool init() = 0;
		virtual bool draw() = 0;
		virtual bool update() = 0;
		
		virtual bool onKey(const KeyData& key){
			return true;
		}
		virtual bool onTouch(const glm::vec2& coord, const glm::vec2& deltacoord){
			return true;
		}
		virtual bool onMove(const glm::vec2& coord, const glm::vec2& deltacoord) {
			return true;
		}
		virtual bool onMouseRight(const glm::vec2& coord){
			return true;
		}
		virtual bool onScroll(const glm::vec2& offset, const float dt){
			return true;
		}
		virtual bool onExit() = 0;

		std::weak_ptr<MainApp> mainApp;
	protected: // Main access level
};
typedef std::shared_ptr<BaseApp> spBaseApp;

};
