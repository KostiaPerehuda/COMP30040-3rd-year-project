#include <iostream>
#include "drone-commander.h"

DroneCommander::DroneCommander(ARDrone* drone)
	: drone(drone), settings({ false, false, false }),
	isSpeedUpdated(false), stabilizer(nullptr), speed()
{
	switchCamera();
}

DroneCommander::DroneCommander(ARDrone* drone, SpeedController* stabilizer)
	: drone(drone), settings({ false, false, false }),
	isSpeedUpdated(false), stabilizer(stabilizer), speed()
{
	switchCamera();
}

SpeedController* DroneCommander::getStabilizer()
{
	return stabilizer;
}

void DroneCommander::setStabilizer(SpeedController* stabilizerIn)
{
	stabilizer = stabilizerIn;
}

vec4f DroneCommander::toMove3DFormat(vec4f speed)
{
	// x = back-front
	// y = up-down
	// z = left-right
	// r = rotation

	if (settings.dualRate) return {
		speed.x *  5.0f,
		speed.z * -5.0f,
		speed.y *  1.0f,
		speed.r * -2.0f,
	};
	else return {
		speed.x *  2.5f,
		speed.z * -2.5f,
		speed.y *  0.5f,
		speed.r * -1.0f,
	};
}

vec4f DroneCommander::toTilt3DFormat(vec4f speed, bool dualRate)
{
	// x = back-front
	// y = up-down
	// z = left-right
	// r = rotation

	if (settings.dualRate || dualRate) return {
		speed.z,
		speed.x,
		speed.y,
		speed.r,
	};
	else return {
		speed.z * 0.5f,
		speed.x * 0.5f,
		speed.y * 0.5f,
		speed.r * 0.5f,
	};
}

bool DroneCommander::isSpeedNonZero()
{
	return speed.x != 0.0f || speed.y != 0.0f || speed.z != 0.0f || speed.r != 0.0f;
}

void DroneCommander::takeoff()
{
	drone->takeoff();

	if (settings.autoStabilization && settings.cameraMode == true && stabilizer) {
		enableStabilizer();
	}
}

void DroneCommander::land()
{
	drone->landing();

	if (stabilizer) stabilizer->disable();

	speed = { 0.0f, 0.0f, 0.0f, 0.0f };
	isSpeedUpdated = false;
}

void DroneCommander::takeoffOrLand()
{
	drone->onGround() ? takeoff() : land();
}

void DroneCommander::switchCamera()
{
	if (settings.cameraMode == true && stabilizer && stabilizer->isEnabled()) {
		//std::cout << "CANNOT switch to FRONT Camera while Stabilization is ENABLED! Disable stabilization first!\n";
		//return;

		// disable satbilization to switch to front camera
		disableStabilizer();
	}

	settings.cameraMode = !settings.cameraMode;
	drone->setCamera(settings.cameraMode);
	std::cout << "Switched to " << (settings.cameraMode ? "BOTTOM" : "FRONT") << " Camera!\n";
}

void DroneCommander::switchRate()
{
	settings.dualRate = !settings.dualRate;
	std::cout << "Dual rate " << (settings.dualRate ? "ENABLED" : "DISABLED") << "!\n";
}

// pointer unsafe!!!
void DroneCommander::enableStabilizer()
{
	stabilizer->enable();
	std::cout << "Stablilzation ENABLED!\n";
}

// pointer unsafe!!!
void DroneCommander::disableStabilizer()
{
	stabilizer->disable();
	std::cout << "Stablilzation DISABLED!\n";

	// enable native stablilization if no motion requested by user
	if (!isSpeedUpdated) {
		speed = { 0.0f, 0.0f, 0.0f, 0.0f };
		isSpeedUpdated = true;
	}
}

void DroneCommander::enableOrDisableStabilization()
{
	if (!stabilizer) {
		std::cout << "CANNOT enable or disable stabilization! No stabilization algorithm is detected!\n";
		return;
	}

	if (stabilizer->isEnabled()) {
		disableStabilizer();
	}
	else if (settings.cameraMode == true) {
		enableStabilizer();
	}
	else {
		std::cout << "CANNOT ENABLE Stabilization! Switch to BOTTOM Camera first!\n";
	}
}

void DroneCommander::shutdown()
{
	land();
	if (AbstractManager* app = getApplicaton()) app->stop();
}

void DroneCommander::onButtonA(bool pressed)
{
	if (pressed) return;
	lock(); land(); unlock();
}

void DroneCommander::onButtonB(bool pressed)
{
	if (pressed) return;
	lock(); takeoffOrLand(); unlock();
}

void DroneCommander::onButtonX(bool pressed)
{
	if (pressed) return;
	lock(); switchCamera(); unlock();
}

void DroneCommander::onButtonY(bool pressed)
{
	if (pressed) return;
	lock(); switchRate(); unlock();
}

void DroneCommander::onLeftShoulder(bool pressed)
{
	//if (pressed) return;
	//lock(); enableOrDisableStabilization(); unlock();
}

void DroneCommander::onRightShoulder(bool pressed)
{
	if (pressed) return;
	lock(); enableOrDisableStabilization(); unlock();
}

void DroneCommander::onLeftTrigger(float value)
{
	if (value != 1.0f) return;
	lock(); shutdown(); unlock();
}

void DroneCommander::onLeftThumb(float x, float y)
{
	// x = back-front
	// y = up-down
	// z = left-right
	// r = rotation

	lock();

	if (drone->onGround()) {
		unlock();
		return;
	}

	speed.y = y;
	speed.r = x;

	isSpeedUpdated = true;

	if (stabilizer && stabilizer->isEnabled() && isSpeedNonZero()) {
		disableStabilizer();
	}

	unlock();
}

void DroneCommander::onRightThumb(float x, float y)
{
	// x = back-front
	// y = up-down
	// z = left-right
	// r = rotation

	lock();

	if (drone->onGround()) {
		unlock();
		return;
	}

	speed.x = y;
	speed.z = x;

	isSpeedUpdated = true;

	if (stabilizer && stabilizer->isEnabled() && isSpeedNonZero()) {
		disableStabilizer();
	}

	unlock();
}

bool DroneCommander::poll()
{
	lock();

	if (drone->onGround()) {
		unlock();
		return false;
	}

	if (stabilizer && stabilizer->isEnabled()) {

		if (stabilizer->isUpdated()) {
			vec4f v = toTilt3DFormat(stabilizer->getSpeed(), true);
			drone->tilt3D(v.x, v.y, v.z, v.r);
			//std::cout << " updated" << std::endl;
		}

	}
	else if (isSpeedUpdated) {

		isSpeedUpdated = false;

		if (settings.autoStabilization && settings.cameraMode == true && stabilizer && !isSpeedNonZero()) {
			drone->tilt3D(0, 0, 0, 0);
			enableStabilizer();
		}
		else {
			vec4f v = toMove3DFormat(speed);
			drone->move3D(v.x, v.y, v.z, v.r);
		}

	}

	unlock();
	return true;
}
