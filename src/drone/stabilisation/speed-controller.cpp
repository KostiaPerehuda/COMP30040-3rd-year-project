#include "speed-controller.h"

#include <iostream>
#include <cmath>

using Configuration::PidConfig;
using Configuration::pidConfig;
using Configuration::pidConfigAdditive;

SpeedController::SpeedController(bool additive, bool useMagnitude)
	: additive(additive), useMagnitude(useMagnitude), config(nullptr),
	       vx(0, 0, 0, 1, -1, 0, 0.5), vy(0, 0, 0, 1, -1, 0, 0.5),
	       vz(0, 0, 0, 1, -1, 0, 0.5), vr(0, 0, 0, 1, -1, 0, 0.5),
	magnitude(0, 0, 0, 1, -1, 0, 0.5),
	timer(), speed(), error(), enabled(false), updated(false),
	errorVisualizer("Error"),
	speedVisualizer("PID Output", -0.1f, 0.1f, -0.1f, 0.1f)
{
	setConfig(additive ? &pidConfigAdditive : &pidConfig);
}

bool SpeedController::update(vec4f feedback)
{
	lock();

	if (!enabled) {
		unlock();
		return false;
	}

	vec4f oldSpeed = { speed.x, speed.y, speed.z, speed.r };

	if (additive) {
		error.x += feedback.x;
		error.y += feedback.y;
		error.z += feedback.z;
		error.r += feedback.r;
	}
	else {
		error = feedback;
	}

	float dt = timer.loop();

	if (useMagnitude) {
		speed = project(magnitude.update(0.0f, getMagnitude(error), dt), error);

		vx.update(0.0f, error.x, dt);
		vy.update(0.0f, error.y, dt);
		vz.update(0.0f, error.z, dt);
		vr.update(0.0f, error.r, dt);
	}
	else {
		speed = {
			vx.update(0.0f, error.x, dt),
			vy.update(0.0f, error.y, dt),
			vz.update(0.0f, error.z, dt),
			vr.update(0.0f, error.r, dt),
		};

		magnitude.update(0.0f, getMagnitude(error), dt);
	}

	updated = oldSpeed.x != speed.x || oldSpeed.y != speed.y ||
		oldSpeed.z != speed.z || oldSpeed.r != speed.r;

	speedVisualizer.draw(speed.z, speed.x);
	errorVisualizer.draw(error.z, error.x);

	unlock();
	return updated;
}

float SpeedController::getMagnitude(vec4f v)
{
	return std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.r * v.r);
}

vec4f SpeedController::project(float magnitude, vec4f v)
{
	float scale = magnitude / getMagnitude(v);
	return {
		v.x * scale,
		v.y * scale,
		v.z * scale,
		v.r * scale,
	};
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
	magnitude.reset();
	timer.loop();
}

void SpeedController::onDpadUp(bool pressed)
{
	if (pressed) return;
	config->Kp += config->KpSensitivity;
	setKp(config->Kp);
}

void SpeedController::onDpadDown(bool pressed)
{
	if (pressed) return;
	config->Kp -= config->KpSensitivity;
	setKp(config->Kp);
}

void SpeedController::onDpadLeft(bool pressed)
{
	if (pressed) return;
	config->Ki -= config->KiSensitivity;
	setKi(config->Ki);
}

void SpeedController::onDpadRight(bool pressed)
{
	if (pressed) return;
	config->Ki += config->KiSensitivity;
	setKi(config->Ki);
}

void SpeedController::onStart(bool pressed)
{
	if (pressed) return;
	config->Kd += config->KdSensitivity;
	setKd(config->Kd);
}

void SpeedController::onBack(bool pressed)
{
	if (pressed) return;
	config->Kd -= config->KdSensitivity;
	setKd(config->Kd);
}

void SpeedController::onLeftThumb(bool pressed)
{
	if (pressed) return;
	useMagnitude = !useMagnitude;
	std::cout << (useMagnitude ? "Use Magnitude!" : "Do Not Use Magnitude!") << std::endl;
}

void SpeedController::onRightTrigger(float value)
{
	if (value != 1.0f) return;
	additive = !additive;
	std::cout << "Set Mode to: " << (additive ? "ADDITIVE" : "INSTANTANEOUS") << std::endl;

	setConfig(additive ? &pidConfigAdditive : &pidConfig);
}

void SpeedController::setKp(float Kp)
{
	vx.setKp(Kp);
	vy.setKp(Kp);
	vz.setKp(Kp);
	vr.setKp(Kp);
	magnitude.setKp(Kp);
	std::cout << "Kp set to: " << config->Kp << std::endl;
}

void SpeedController::setKi(float Ki)
{
	vx.setKi(Ki);
	vy.setKi(Ki);
	vz.setKi(Ki);
	vr.setKi(Ki);
	magnitude.setKi(Ki);
	std::cout << "Ki set to: " << config->Ki << std::endl;
}

void SpeedController::setKd(float Kd)
{
	vx.setKd(Kd);
	vy.setKd(Kd);
	vz.setKd(Kd);
	vr.setKd(Kd);
	magnitude.setKd(Kd);
	std::cout << "Kd set to: " << config->Kd << std::endl;
}

void SpeedController::setTau(float tau)
{
	vx.setTau(tau);
	vy.setTau(tau);
	vz.setTau(tau);
	vr.setTau(tau);
	magnitude.setTau(tau);
	std::cout << "Tau set to: " << config->tau << std::endl;
}

void SpeedController::set(float Kp, float Ki, float Kd, float tau)
{
	setKp(Kp);
	setKi(Ki);
	setKd(Kd);
	setTau(tau);
}

void SpeedController::setConfig(Configuration::PidConfig* configIn)
{
	config = configIn;
	set(config->Kp, config->Ki, config->Kd, config->tau);
}
