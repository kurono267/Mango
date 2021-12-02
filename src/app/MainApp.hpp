//
//  MainAppBase.hpp
//  MangoIOS
//
//  Created by kurono267 on 06.02.2019.
//  Copyright © 2019 kurono267. All rights reserved.
//

#pragma once

#include <memory>
#include "../third_party.hpp"
#include <iostream>

namespace mango {

struct KeyData {
    KeyData();
    KeyData(int _key, int _scancode, int _action, int _mods);
    KeyData(const KeyData& state) = default;
    virtual ~KeyData() = default;

    int key; int scancode; int action; int mods;
};

class BaseApp;
typedef std::shared_ptr<BaseApp> spBaseApp;

class MainApp {
    public:
        MainApp() = default;
        virtual ~MainApp(){
            std::cout << "MainApp Destructor" << std::endl;
        }
    
        virtual void setBaseApp(spBaseApp app) = 0;
    
        [[nodiscard]] virtual const KeyData keyState() const = 0;
    
        virtual void exit() = 0;
        // Create window and setup
        virtual glm::ivec2 wndSize() = 0; // Return window size
        virtual glm::ivec2 frameSize() = 0;
    
        virtual void run() = 0;
        virtual bool is() = 0;
    
        virtual void* window() = 0; // Internal window
	protected:
		spBaseApp _app;
};

typedef std::shared_ptr<MainApp> spMainApp;

}
