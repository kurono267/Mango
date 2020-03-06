//
//  MainApp.cpp
//  MangoIOS
//
//  Created by kurono267 on 07.02.2019.
//  Copyright © 2019 kurono267. All rights reserved.
//

#include "MainApp.hpp"

namespace mango {

KeyData::KeyData() : key(0), scancode(0), action(0), mods(0) {}
KeyData::KeyData(int _key, int _scancode, int _action, int _mods) : key(_key), scancode(_scancode), action(_action), mods(_mods) {}

}
