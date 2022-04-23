#include "speed-controller.h"

#include <iostream>

SpeedController::SpeedController(float Kp, float Ki, float Kd, float tau, bool additive)
	: vx(Kp, Ki, Kd, 1, -1, tau), vy(Kp, Ki, Kd, 1, -1, tau),
	vz(Kp, Ki, Kd, 1, -1, tau), vr(Kp, Ki, Kd, 1, -1, tau),
	timer(), speed(), error(), additive(additive), enabled(false), updated(false),
	errorVisualizer("Error"), speedVisualizer("PID Output") {}

bool SpeedController::update(vec4f feedback)
{
	lock();

	if (!enabled) {
		unlock();
		return false;
	}

	vec4f oldSpeed = { speed.x, speed.y, speed.z, speed.r };

	float dt = timer.loop();

	if (additive) {
		error.x += feedback.x;
		error.y += feedback.y;
		error.z += feedback.z;
		error.r += feedback.r;
	}
	else {
		error = feedback;
	}

	speed = {
		vx.update(0.0f, error.x, dt),
		vy.update(0.0f, error.y, dt),
		vz.update(0.0f, error.z, dt),
		vr.update(0.0f, error.r, dt),
	};

	updated = oldSpeed.x != speed.x || oldSpeed.y != speed.y ||
		oldSpeed.z != speed.z || oldSpeed.r != speed.r;

	//if (updated) std::cout << "updated" << std::endl;

	speedVisualizer.draw(speed.z, speed.x);
	errorVisualizer.draw(error.z, error.x);

	unlock();
	return updated;
}

bool SpeedController::isUpdated()
{
	return updated;
}

vec4f SpeedController::getSpeed()
{
	lock();
	updated = false;
	unlock();
	return speed;
}

void SpeedController::enable()
{
	lock();
	enabled = true;
	resetSelf();
	unlock();
}

void SpeedController::disable()
{
	lock();
	enabled = false;
	speedVisualizer.draw(0, 0);
	errorVisualizer.draw(0, 0);
	unlock();
}

bool SpeedController::isEnabled()
{
	return enabled;
}

void SpeedController::reset()
{
	lock();
	resetSelf();
	unlock();
}

void SpeedController::resetSelf()
{
	updated = false;
	speed = vec4f();
	error = vec4f();
	speedVisualizer.draw(0, 0);
	errorVisualizer.draw(0, 0);
	vx.reset();
	vy.reset();
	vz.reset();
	vz.reset();
	timer.loop();
}

void SpeedController::onDpadUp(bool pressed)
{
	if (pressed) return;
	vx.setKp(vx.getKp() + 0.025f);
	vy.setKp(vy.getKp() + 0.025f);
	vz.setKp(vz.getKp() + 0.025f);
	vr.setKp(vr.getKp() + 0.025f);
	std::cout << "Kp set to: " << vx.getKp() << std::endl;
}

void SpeedController::onDpadDown(bool pressed)
{
	if (pressed) return;
	vx.setKp(vx.getKp() - 0.025f);
	vy.setKp(vy.getKp() - 0.025f);
	vz.setKp(vz.getKp() - 0.025f);
	vr.setKp(vr.getKp() - 0.025f);
	std::cout << "Kp set to: " << vx.getKp() << std::endl;
}

void SpeedController::onDpadLeft(bool pressed)
{
	if (pressed) return;
	vx.setKi(vx.getKi() - 0.025f);
	vy.setKi(vy.getKi() - 0.025f);
	vz.setKi(vz.getKi() - 0.025f);
	vr.setKi(vr.getKi() - 0.025f);
	std::cout << "Ki set to: " << vx.getKi() << std::endl;
}

void SpeedController::onDpadRight(bool pressed)
{
	if (pressed) return;
	vx.setKi(vx.getKi() + 0.025f);
	vy.setKi(vy.getKi() + 0.025f);
	vz.setKi(vz.getKi() + 0.025f);
	vr.setKi(vr.getKi() + 0.025f);
	std::cout << "Ki set to: " << vx.getKi() << std::endl;
}

void SpeedController::onStart(bool pressed)
{
	if (pressed) return;
	vx.setKd(vx.getKd() + 0.025f);
	vy.setKd(vy.getKd() + 0.025f);
	vz.setKd(vz.getKd() + 0.025f);
	vr.setKd(vr.getKd() + 0.025f);
	std::cout << "Kd set to: " << vx.getKd() << std::endl;
}

void SpeedController::onBack(bool pressed)
{
	if (pressed) return;
	vx.setKd(vx.getKd() - 0.025f);
	vy.setKd(vy.getKd() - 0.025f);
	vz.setKd(vz.getKd() - 0.025f);
	vr.setKd(vr.getKd() - 0.025f);
	std::cout << "Kd set to: " << vx.getKd() << std::endl;
}

void SpeedController::onRightTrigger(float value)
{
	if (value != 1.0f) return;
	additive = !additive;
	std::cout << "Set Mode to: " << (additive ? "ADDITIVE" : "INSTANTANEOUS") << std::endl;
}
