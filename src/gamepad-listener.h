/*
 * gamepad-listener.h
 *
 *  Created on: Feb 06, 2022
 *      Author: kostia
 */

#ifndef GAMEPAD_LISTENER_H_
#define GAMEPAD_LISTENER_H_


#include "gamepad-listener-interface.h"

class GamepadListener : public GamepadListenerAdapter
{
private:
public:
	void onDpadUp(bool pressed) override;
};


#endif /* GAMEPAD_LISTENER_H_ */