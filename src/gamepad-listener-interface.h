/*
 * gamepad-listener-interface.h
 *
 *  Created on: Feb 06, 2022
 *      Author: kostia
 */

#ifndef GAMEPAD_LISTENER_INTERFACE_H_
#define GAMEPAD_LISTENER_INTERFACE_H_


class GamepadListenerInterface
{
private:
public:
	virtual void onConnectionEstablished() = 0;
	virtual void onConnectionLost() = 0;

	virtual void onDpadUp(bool pressed) = 0;
	virtual void onDpadDown(bool pressed) = 0;
	virtual void onDpadLeft(bool pressed) = 0;
	virtual void onDpadRight(bool pressed) = 0;
	virtual void onStart(bool pressed)= 0;
	virtual void onBack(bool pressed) = 0;
	virtual void onLeftThumb(bool pressed) = 0;
	virtual void onRightThumb(bool pressed) = 0;
	virtual void onLeftShoulder(bool pressed) = 0;
	virtual void onRightShoulder(bool pressed) = 0;
	virtual void onButtonA(bool pressed) = 0;
	virtual void onButtonB(bool pressed) = 0;
	virtual void onButtonX(bool pressed) = 0;
	virtual void onButtonY(bool pressed) = 0;

	virtual void onLeftTrigger(float value) = 0;
	virtual void onRightTrigger(float value) = 0;
	virtual void onLeftThumb(float x, float y) = 0;
	virtual void onRightThumb(float x, float y) = 0;

	virtual ~GamepadListenerInterface() = default;
};

class GamepadListenerAdapter : public GamepadListenerInterface
{
private:
public:
	virtual void onConnectionEstablished() override;
	virtual void onConnectionLost() override;

	virtual void onDpadUp(bool pressed) override;
	virtual void onDpadDown(bool pressed) override;
	virtual void onDpadLeft(bool pressed) override;
	virtual void onDpadRight(bool pressed) override;
	virtual void onStart(bool pressed) override;
	virtual void onBack(bool pressed) override;
	virtual void onLeftThumb(bool pressed) override;
	virtual void onRightThumb(bool pressed) override;
	virtual void onLeftShoulder(bool pressed) override;
	virtual void onRightShoulder(bool pressed) override;
	virtual void onButtonA(bool pressed) override;
	virtual void onButtonB(bool pressed) override;
	virtual void onButtonX(bool pressed) override;
	virtual void onButtonY(bool pressed) override;

	virtual void onLeftTrigger(float value) override;
	virtual void onRightTrigger(float value) override;
	virtual void onLeftThumb(float x, float y) override;
	virtual void onRightThumb(float x, float y) override;

	virtual ~GamepadListenerAdapter() = default;
};


#endif /* GAMEPAD_LISTENER_INTERFACE_H_ */