/*
 * low-pass-filter.cpp
 *
 *  Created on: Feb 19, 2022
 *      Author: kostia
 */

#include "low-pass-filter.h"

LowPassFilter::LowPassFilter()
	: oldTime(std::chrono::milliseconds()), currentTime(std::chrono::milliseconds()),
	oldValue(0.0f), oldFilteredValue(0.0f), tau(0.0f) {}

LowPassFilter::LowPassFilter(float tau)
	: oldTime(std::chrono::milliseconds()), currentTime(std::chrono::milliseconds()),
	oldValue(0.0f), oldFilteredValue(0.0f), tau(tau) {}


void LowPassFilter::setTau(float tauIn)
{
	tau = tauIn;
}

float LowPassFilter::process(float value)
{
	//currentTime std::chrono::system_clock::now().time_since_epoch().count();
	float dt = (float) (currentTime - oldTime).count() / 1000;
	oldTime = currentTime;
	return process(value, dt);
}

float LowPassFilter::process(float value, float dt)
{
	return ((dt) * (value + oldValue) + (2.0f * tau - dt) * (oldFilteredValue)) / (2.0f * tau + dt);
}
