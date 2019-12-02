//
// Created by kurono267 on 2018-12-07.
//

#include "App.hpp"

int main(){
    spMainApp main = MainApp::instance();

    main->create("Cube",1280,720);
    main->createApplication<App>();

    main->run();

    return 0;
}