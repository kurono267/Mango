//
//  MainAppIOS.hpp
//  MangoIOS
//
//  Created by kurono267 on 06.02.2019.
//  Copyright © 2019 kurono267. All rights reserved.
//

#pragma once

#include "MainApp.hpp"

namespace mango {

class MainAppIOS : public MainApp,std::enable_shared_from_this<MainAppIOS> {
    public:
        typedef std::shared_ptr<MainAppIOS> ptr;
        MainAppIOS();
        virtual ~MainAppIOS(){
            std::cout << "MainApp Destructor" << std::endl;
        }

        void setBaseApp(spBaseApp app) override;

        const KeyData glfwKeyState() const override;

        void exit() override;
        // Create window and setup
        glm::ivec2 wndSize() override; // Return window size

        void run() override;

        void nextFrame();
        bool is() override;

        void* window() override; // Internal window

        void create(const int width, const int height, void* view);

        static ptr& instance(){
            static ptr app; // lazy singleton, instantiated on first use
            if(!app)app = std::make_shared<MainAppIOS>();
            return app;
        }
    private:
        spBaseApp _app;

        std::string _title;

        void* _view;
        glm::ivec2 _size;

        bool _isRun;
};

typedef std::shared_ptr<MainAppIOS> spMainAppIOS;

}
