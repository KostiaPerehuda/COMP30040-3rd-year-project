/*
 * gamepad-input-manager.h
 *
 *  Created on: Feb 04, 2022
 *      Author: kostia
 */

#ifndef GAMEPAD_INPUT_MANAGER_H_
#define GAMEPAD_INPUT_MANAGER_H_


#include <mutex>
#include <vector>
#include <windows.h>
#include <xinput.h>
#include "manager-interface.h"
#include "gamepad-listener-interface.h"

class GamepadInputManager : public ManagerInterface
{
private:
	std::mutex m_mutex;
	std::vector<GamepadListenerInterface*> m_gamepadListeners;
	XINPUT_STATE previousState, currentState;
	unsigned char triggerDeadZone;
	short leftThumbDeadZone;
	short rightThumbDeadZone;
	int connectedControllerId;

	int getConnectedControllerId();
	void checkButton(WORD button);
	void checkLeftTrigger();
	void checkRightTrigger();
	void checkLeftThumb();
	void checkRightThumb();

	static float normalize(short value, short lower, short upper);
	static float normailzeWithSign(short value, short lower, short upper);
public:
	GamepadInputManager();
	GamepadInputManager(float thumbDeadZoneIn, float triggerDeadZoneIn);
	void setTriggerDeadZone(float deadZone);
	void setThumbDeadZone(float deadZone);
	void setLeftThumbDeadZone(float deadZone);
	void setRightThumbDeadZone(float deadZone);
	void addGamepadListener(GamepadListenerInterface* listener);
	bool poll() override;
};


#endif /* GAMEPAD_INPUT_MANAGER_H_ */