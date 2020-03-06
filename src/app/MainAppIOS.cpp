//
//  MainAppIOS.cpp
//  MangoIOS
//
//  Created by kurono267 on 06.02.2019.
//  Copyright © 2019 kurono267. All rights reserved.
//

#include "MainAppIOS.hpp"
#include "BaseApp.hpp"

namespace mango {


MainAppIOS::MainAppIOS() : _isRun(false) {

}

void MainAppIOS::setBaseApp(spBaseApp app) {
    _app = app;
}

const KeyData MainAppIOS::glfwKeyState() const {
    return KeyData();
}

void MainAppIOS::exit() {
    _isRun = false;
}

glm::ivec2 MainAppIOS::wndSize() {
    return _size;
}

void MainAppIOS::run() {

}

bool MainAppIOS::is() {
    return _isRun;
}

void *MainAppIOS::window() {
    return _view;
}

void MainAppIOS::create(const int width, const int height, void* view){
    _size = glm::ivec2(width,height);
    _view = view;
    
    _app->mainApp = instance();
    _app->init();
}

void MainAppIOS::nextFrame(){
    _app->update();
    _app->draw();
}

}
