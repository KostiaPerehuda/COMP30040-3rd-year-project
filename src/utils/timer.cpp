#include "timer.h"

Timer::Timer() : oldTime(getTime()), currentTime() {}

std::chrono::milliseconds Timer::getTime()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}

float Timer::loop()
{
	currentTime = getTime();
	deltaTime = (float) (currentTime.count() - oldTime.count()) / 1000;
	oldTime = currentTime;
	return deltaTime;
}

float Timer::get()
{
	currentTime = getTime();
	deltaTime = (float)(currentTime.count() - oldTime.count()) / 1000;
	return deltaTime;
}

void Timer::add(float time)
{
	oldTime = oldTime + std::chrono::milliseconds(static_cast<int>(1000 * time));
}

bool Timer::hasPassed(float time)
{
	currentTime = getTime();
	deltaTime = (float) (currentTime.count() - oldTime.count()) / 1000;
	return deltaTime >= time;
}

bool Timer::addIfHasPassed(float time)
{
	if (!hasPassed(time)) return false;
	
	add(time);
	return true;
}
