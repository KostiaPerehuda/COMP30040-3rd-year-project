/*
 * gamepad-listener-adapter.cpp
 *
 *  Created on: Feb 06, 2022
 *      Author: kostia
 */

#include "gamepad-listener-interface.h"
#include <iostream>

void GamepadListenerAdapter::onConnectionEstablished() { /*std::cout << "Connection to xbox controller established" << std::endl;*/ }

void GamepadListenerAdapter::onConnectionLost() { /*std::cout << "Connection to xbox controller lost" << std::endl;*/ }

void GamepadListenerAdapter::onDpadUp(bool pressed) { /*std::cout << "Dpad Up " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onDpadDown(bool pressed) { /*std::cout << "Dpad Down " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onDpadLeft(bool pressed) { /*std::cout << "Dpad Left " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onDpadRight(bool pressed) { /*std::cout << "Dpad Right " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onStart(bool pressed) { /*std::cout << "Start Button " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onBack(bool pressed) { /*std::cout << "Back Button " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onLeftThumb(bool pressed) { /*std::cout << "Left Thumb " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onRightThumb(bool pressed) { /*std::cout << "Right Thumb " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onLeftShoulder(bool pressed) { /*std::cout << "Left Shoulder " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onRightShoulder(bool pressed) { /*std::cout << "Right Shoulder " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onButtonA(bool pressed) { /*std::cout << "Button A " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onButtonB(bool pressed) { /*std::cout << "Button B " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onButtonX(bool pressed) { /*std::cout << "Button X " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onButtonY(bool pressed) { /*std::cout << "Button Y " << (pressed ? "pressed" : "released") << std::endl;*/ }

void GamepadListenerAdapter::onLeftTrigger(float value) { /*std::cout << "Left Trigger " << value << std::endl;*/ }

void GamepadListenerAdapter::onRightTrigger(float value) { /*std::cout << "Right Trigger " << value << std::endl;*/ }

void GamepadListenerAdapter::onLeftThumb(float x, float y) { /*std::cout << "Left Thumb (" << x << ", " << y << ")" << std::endl;*/ }

void GamepadListenerAdapter::onRightThumb(float x, float y) { /*std::cout << "Right Thumb (" << x << ", " << y << ")" << std::endl;*/ }
