//
// Created by kurono267 on 2018-12-07.
//

#include "App.hpp"
#include <app/MainAppGLFW.hpp>

int main(){
	auto main = MainAppGLFW::instance();

	main->create("GLTF",1280,720);
	main->setBaseApp(std::make_shared<App>(main));

	main->run();

    return 0;
}