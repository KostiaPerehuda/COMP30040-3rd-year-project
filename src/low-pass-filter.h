/*
 * low-pass-filter.h
 *
 *  Created on: Feb 19, 2022
 *      Author: kostia
 */

#ifndef LOW_PASS_FILTER_H_
#define LOW_PASS_FILTER_H_


#include <chrono>

class LowPassFilter
{
private:
	float oldValue, oldFilteredValue;
	float tau;

	std::chrono::milliseconds oldTime, currentTime;
public:
	LowPassFilter();
	LowPassFilter(float tau);
	
	void setTau(float tau);

	float process(float value);
	float process(float value, float dt);
};


#endif /* LOW_PASS_FILTER_H_ */