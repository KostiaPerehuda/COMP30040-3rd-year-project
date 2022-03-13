/*
 * timer.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: kostia
 */

#include "timer.h"

Timer::Timer() : oldTime(getTime()), currentTime() {}

std::chrono::milliseconds Timer::getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

float Timer::get()
{
	currentTime = getTime();
	deltaTime = (float) (currentTime.count() - oldTime.count()) / 1000;
	oldTime = currentTime;
	return deltaTime;
}
