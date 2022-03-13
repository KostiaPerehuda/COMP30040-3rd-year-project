/*
 * gamepad-listener.h
 *
 *  Created on: Jan 30, 2022
 *      Author: kostia
 */

#include "gamepad-input-manager.h"
#include "gamepad-listener.h"
#include <iostream>

void GamepadListener::onDpadUp(bool pressed)
{
    std::cout << "Dpad Up " << (pressed ? "pressed" : "released") << std::endl;
}

//int main() {
//    GamepadListener l;
//    GamepadInputManager m;
//    m.addGamepadListener(&l);
//    while (1) {
//        m.poll();
//    }
//}
