#include "gamepad-input-manager.h"

#include <iostream>
#include <winerror.h>
#include <minwinbase.h>

GamepadInputManager::GamepadInputManager()
	//: connectedControllerId(-1), triggerDeadZone(XINPUT_GAMEPAD_TRIGGER_THRESHOLD-1),
	//leftThumbDeadZone(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE-1), rightThumbDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE-1)
	//: connectedControllerId(-1), triggerDeadZone(0), leftThumbDeadZone(0), rightThumbDeadZone(0)
	: connectedControllerId(-1), triggerDeadZone(0),
	leftThumbDeadZone(XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE-1), rightThumbDeadZone(XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE-1)
{
	ZeroMemory(&previousState, sizeof(XINPUT_STATE));
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
}

GamepadInputManager::GamepadInputManager(float thumbDeadZone, float triggerDeadZone)
	: connectedControllerId(-1)
{
	ZeroMemory(&previousState, sizeof(XINPUT_STATE));
	ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	setThumbDeadZone(thumbDeadZone);
	setTriggerDeadZone(triggerDeadZone);
}

int GamepadInputManager::getConnectedControllerId()
{
	XINPUT_STATE state;

	for (DWORD i = 0; i < XUSER_MAX_COUNT; i++) {
		if (XInputGetState(i, &state) == ERROR_SUCCESS) {
			// found controller
			connectedControllerId = i;

			lock();
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onConnectionEstablished();
			unlock();

			return i;
		}
	}

	return -1;
}

void GamepadInputManager::checkButton(WORD button)
{
	WORD pressed;
	switch (button) {
	case XINPUT_GAMEPAD_DPAD_UP:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onDpadUp(pressed);
		break;
	case XINPUT_GAMEPAD_DPAD_DOWN:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onDpadDown(pressed);
		break;
	case XINPUT_GAMEPAD_DPAD_LEFT:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onDpadLeft(pressed);
		break;
	case XINPUT_GAMEPAD_DPAD_RIGHT:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onDpadRight(pressed);
		break;
	case XINPUT_GAMEPAD_START:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onStart(pressed);
		break;
	case XINPUT_GAMEPAD_BACK:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onBack(pressed);
		break;
	case XINPUT_GAMEPAD_LEFT_THUMB:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onLeftThumb(pressed);
		break;
	case XINPUT_GAMEPAD_RIGHT_THUMB:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onRightThumb(pressed);
		break;
	case XINPUT_GAMEPAD_LEFT_SHOULDER:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onLeftShoulder(pressed);
		break;
	case XINPUT_GAMEPAD_RIGHT_SHOULDER:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onRightShoulder(pressed);
		break;
	case XINPUT_GAMEPAD_A:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onButtonA(pressed);
		break;
	case XINPUT_GAMEPAD_B:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onButtonB(pressed);
		break;
	case XINPUT_GAMEPAD_X:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onButtonX(pressed);
		break;
	case XINPUT_GAMEPAD_Y:
		if ((pressed = currentState.Gamepad.wButtons & button) != (previousState.Gamepad.wButtons & button))
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onButtonY(pressed);
		break;
	}
}

void GamepadInputManager::checkLeftTrigger()
{
	if (triggerDeadZone == 0) {
		if (currentState.Gamepad.bLeftTrigger != previousState.Gamepad.bLeftTrigger)
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onLeftTrigger((float)currentState.Gamepad.bLeftTrigger / UCHAR_MAX);
	}
	else {
		if (currentState.Gamepad.bLeftTrigger <= triggerDeadZone)
			currentState.Gamepad.bLeftTrigger = 0;

		if (currentState.Gamepad.bLeftTrigger != previousState.Gamepad.bLeftTrigger) {
			float trigger = normalize(currentState.Gamepad.bLeftTrigger, triggerDeadZone, UCHAR_MAX);
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onLeftTrigger(trigger);
		}
	}
}

void GamepadInputManager::checkRightTrigger()
{
	if (triggerDeadZone == 0) {
		if (currentState.Gamepad.bRightTrigger != previousState.Gamepad.bRightTrigger)
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onRightTrigger((float)currentState.Gamepad.bRightTrigger / UCHAR_MAX);
	}
	else {
		if (currentState.Gamepad.bRightTrigger <= triggerDeadZone)
			currentState.Gamepad.bRightTrigger = 0;

		if (currentState.Gamepad.bRightTrigger != previousState.Gamepad.bRightTrigger) {
			float trigger = normalize(currentState.Gamepad.bRightTrigger, triggerDeadZone, UCHAR_MAX);
			for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
				m_gamepadListeners[i]->onRightTrigger(trigger);
		}
	}
	
	
}

inline void GamepadInputManager::checkLeftThumb()
{
	// clamp x value
	if (currentState.Gamepad.sThumbLX <= leftThumbDeadZone && currentState.Gamepad.sThumbLX >= -leftThumbDeadZone)
		currentState.Gamepad.sThumbLX = 0;
	else if (currentState.Gamepad.sThumbLX == SHRT_MIN) currentState.Gamepad.sThumbLX = -SHRT_MAX;

	// clamp y value
	if (currentState.Gamepad.sThumbLY <= leftThumbDeadZone && currentState.Gamepad.sThumbLY >= -leftThumbDeadZone)
		currentState.Gamepad.sThumbLY = 0;
	else if (currentState.Gamepad.sThumbLY == SHRT_MIN) currentState.Gamepad.sThumbLY = -SHRT_MAX;

	// check if changed
	if (currentState.Gamepad.sThumbLX != previousState.Gamepad.sThumbLX || currentState.Gamepad.sThumbLY != previousState.Gamepad.sThumbLY) {
		float x = normailzeWithSign(currentState.Gamepad.sThumbLX, leftThumbDeadZone, SHRT_MAX);
		float y = normailzeWithSign(currentState.Gamepad.sThumbLY, leftThumbDeadZone, SHRT_MAX);
		for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
			m_gamepadListeners[i]->onLeftThumb(x, -y);
	}
}

inline void GamepadInputManager::checkRightThumb()
{
	// clamp x value
	if (currentState.Gamepad.sThumbRX <= rightThumbDeadZone && currentState.Gamepad.sThumbRX >= -rightThumbDeadZone)
		currentState.Gamepad.sThumbRX = 0;
	else if (currentState.Gamepad.sThumbRX == SHRT_MIN) currentState.Gamepad.sThumbRX = -SHRT_MAX;

	// clamp y value
	if (currentState.Gamepad.sThumbRY <= rightThumbDeadZone && currentState.Gamepad.sThumbRY >= -rightThumbDeadZone)
		currentState.Gamepad.sThumbRY = 0;
	else if (currentState.Gamepad.sThumbRY == SHRT_MIN) currentState.Gamepad.sThumbRY = -SHRT_MAX;

	// check if changed
	if (currentState.Gamepad.sThumbRX != previousState.Gamepad.sThumbRX || currentState.Gamepad.sThumbRY != previousState.Gamepad.sThumbRY) {
		float x = normailzeWithSign(currentState.Gamepad.sThumbRX, rightThumbDeadZone, SHRT_MAX);
		float y = normailzeWithSign(currentState.Gamepad.sThumbRY, rightThumbDeadZone, SHRT_MAX);
		for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
			m_gamepadListeners[i]->onRightThumb(x, -y);
	}
}

float GamepadInputManager::normalize(short value, short lower, short upper)
{
		if (value <= lower) return 0;
		if (value >= upper) return 1;
		return (float)(value - lower) / (upper - lower);
}

float GamepadInputManager::normailzeWithSign(short value, short lower, short upper)
{
	return (value < 0) ? -normalize(-value, lower, upper) : normalize(value, lower, upper);
}

void GamepadInputManager::setTriggerDeadZone(float deadZone)
{
	lock();
	triggerDeadZone = deadZone == 0 ? 0 : static_cast<unsigned char>(UCHAR_MAX * deadZone);
	unlock();
}

void GamepadInputManager::setThumbDeadZone(float deadZone)
{
	lock();
	leftThumbDeadZone  = deadZone == 0 ? 0 : static_cast<short>(SHRT_MAX * deadZone);
	rightThumbDeadZone = deadZone == 0 ? 0 : static_cast<short>(SHRT_MAX * deadZone);
	unlock();
}

void GamepadInputManager::setLeftThumbDeadZone(float deadZone)
{
	lock();
	leftThumbDeadZone  = deadZone == 0 ? 0 : static_cast<short>(SHRT_MAX * deadZone);
	unlock();
}

void GamepadInputManager::setRightThumbDeadZone(float deadZone)
{
	lock();
	rightThumbDeadZone = deadZone == 0 ? 0 : static_cast<short>(SHRT_MAX * deadZone);
	unlock();
}

void GamepadInputManager::addGamepadListener(GamepadListenerInterface* listener)
{
	lock();
	m_gamepadListeners.push_back(listener);
	if (connectedControllerId != -1) listener->onConnectionEstablished();
	unlock();
}

bool GamepadInputManager::poll()
{
	// if no controller connected, find it
	if ((connectedControllerId == -1) && (getConnectedControllerId() == -1)) {
		return false;
	}

	ZeroMemory(&currentState, sizeof(XINPUT_STATE));

	// get new packet from controller
	if (XInputGetState(connectedControllerId, &currentState) != ERROR_SUCCESS) {
		// lost controller
		connectedControllerId = -1;
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));

		lock();
		for (std::size_t i = 0; i < m_gamepadListeners.size(); ++i)
			m_gamepadListeners[i]->onConnectionLost();
		unlock();

		return false;
	}

	// check if the received packet is an old packet and if so, don't procced
	if (previousState.dwPacketNumber == currentState.dwPacketNumber) {
		return true;
	}

	// lock listeners
	lock();

	// check button presses
	for (unsigned int i = 0; i < sizeof(DWORD) * 8; i++)
		checkButton(1 << i);

	// check triggers
	checkLeftTrigger();
	checkRightTrigger();

	// check thumb sticks
	checkLeftThumb();
	checkRightThumb();

	// unlock listeners
	unlock();

	previousState = currentState;
	return true;
}
