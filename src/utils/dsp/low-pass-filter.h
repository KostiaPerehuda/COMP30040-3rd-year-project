#ifndef LOW_PASS_FILTER_H_
#define LOW_PASS_FILTER_H_


#include <chrono>

#include "utils/vector.h"

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
	float getTau();

	float process(float value);
	float process(float value, float dt);
};


class LowPassFilter2f
{
private:
	LowPassFilter filterX, filterY;

public:
	LowPassFilter2f();
	LowPassFilter2f(float tau);
	LowPassFilter2f(vec2f tau);

	void setTau(float tau);
	void setTau(vec2f tau);
	vec2f getTau();

	vec2f process(vec2f value);
	vec2f process(vec2f value, float dt);
};


class LowPassFilter3f
{
private:
	LowPassFilter filterX, filterY, filterZ;

public:
	LowPassFilter3f();
	LowPassFilter3f(float tau);
	LowPassFilter3f(vec3f tau);

	void setTau(float tau);
	void setTau(vec3f tau);
	vec3f getTau();

	vec3f process(vec3f value);
	vec3f process(vec3f value, float dt);
};


class LowPassFilter4f
{
private:
	LowPassFilter filterX, filterY, filterZ, filterR;

public:
	LowPassFilter4f();
	LowPassFilter4f(float tau);
	LowPassFilter4f(vec4f tau);

	void setTau(float tau);
	void setTau(vec4f tau);
	vec4f getTau();

	vec4f process(vec4f value);
	vec4f process(vec4f value, float dt);
};


#endif /* LOW_PASS_FILTER_H_ */
