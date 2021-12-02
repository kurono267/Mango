#pragma once

#include <functional>
#include <memory>
#include <iostream>

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

		const KeyData keyState() const override;

		void exit() override;
		// Create window and setup
		void create(const std::string& title,const int width,const int height);
		glm::ivec2 wndSize() override; // Return window size
		glm::ivec2 frameSize() override;
		
		void vsync(bool on);

		void run() override;
		bool is() override;

		void* window() override;
	protected:
		void onKey(int key, int scancode, int action, int mods);
		void onMouseBtn(int button, int action, int mods);
		void onMousePos(double x, double y);
		void onScroll(double x, double y);
		void onResize(int width, int height);

		// Current statement
		glm::vec2 _prevMousePos;
		std::chrono::system_clock::time_point _prevMouseTime;

		std::string _title;

		KeyData _lastKey;
		void* _window;

		bool _isRun;
};

};
