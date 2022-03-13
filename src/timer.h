/*
 * timer.h
 *
 *  Created on: Feb 13, 2022
 *      Author: kostia
 */

#ifndef TIMER_H_
#define TIMER_H_


#include <chrono>

class Timer
{
private:
	float deltaTime;
	std::chrono::milliseconds oldTime, currentTime;

	static std::chrono::milliseconds getTime();
public:
	Timer();

	float get();
};


#endif /* TIMER_H_ */