#ifndef GAMEPAD_INPUT_MANAGER_H_
#define GAMEPAD_INPUT_MANAGER_H_


#include <vector>

#include <windows.h>
#include <xinput.h>

#include "core/managers/abstract-manager.h"
#include "core/listeners/gamepad-listener-interface.h"

#include "utils/lockable-object.h"

class GamepadInputManager : public AbstractManager, private LockableObject
{
private:
	std::vector<GamepadListenerInterface*> m_gamepadListeners;
	XINPUT_STATE previousState, currentState;
	unsigned char triggerDeadZone;
	short leftThumbDeadZone;
	short rightThumbDeadZone;
	int connectedControllerId;

public:
	GamepadInputManager();
	GamepadInputManager(float thumbDeadZone, float triggerDeadZone);

private:
	int getConnectedControllerId();
	void checkButton(WORD button);
	void checkLeftTrigger();
	void checkRightTrigger();
	void checkLeftThumb();
	void checkRightThumb();

private:
	static float normalize(short value, short lower, short upper);
	static float normailzeWithSign(short value, short lower, short upper);

public:
	void setTriggerDeadZone(float deadZone);
	void setThumbDeadZone(float deadZone);
	void setLeftThumbDeadZone(float deadZone);
	void setRightThumbDeadZone(float deadZone);
	void addGamepadListener(GamepadListenerInterface* listener);
	bool poll() override;
};


#endif /* GAMEPAD_INPUT_MANAGER_H_ */
