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

float LowPassFilter::getTau()
{
	return tau;
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
	oldFilteredValue = ((dt) * (value + oldValue) + (2.0f * tau - dt) * (oldFilteredValue)) / (2.0f * tau + dt);
	oldValue = value;
	return oldFilteredValue;
}



LowPassFilter2f::LowPassFilter2f() : filterX(), filterY() {}

LowPassFilter2f::LowPassFilter2f(float tau) : filterX(tau), filterY(tau) {}

LowPassFilter2f::LowPassFilter2f(vec2f tau) : filterX(tau.x), filterY(tau.y) {}

void LowPassFilter2f::setTau(float tau)
{
	filterX.setTau(tau);
	filterY.setTau(tau);
}

void LowPassFilter2f::setTau(vec2f tau)
{
	filterX.setTau(tau.x);
	filterY.setTau(tau.y);
}

vec2f LowPassFilter2f::getTau()
{
	return {
		filterX.getTau(),
		filterY.getTau(),
	};
}

vec2f LowPassFilter2f::process(vec2f value)
{
	return { filterX.process(value.x), filterY.process(value.y) };
}

vec2f LowPassFilter2f::process(vec2f value, float dt)
{
	return { filterX.process(value.x, dt), filterY.process(value.y, dt) };
}



LowPassFilter3f::LowPassFilter3f() : filterX(), filterY(), filterZ() {}

LowPassFilter3f::LowPassFilter3f(float tau) : filterX(tau), filterY(tau), filterZ(tau) {}

LowPassFilter3f::LowPassFilter3f(vec3f tau) : filterX(tau.x), filterY(tau.y), filterZ(tau.z) {}

void LowPassFilter3f::setTau(float tau)
{
	filterX.setTau(tau);
	filterY.setTau(tau);
	filterZ.setTau(tau);
}

void LowPassFilter3f::setTau(vec3f tau)
{
	filterX.setTau(tau.x);
	filterY.setTau(tau.y);
	filterZ.setTau(tau.z);
}

vec3f LowPassFilter3f::getTau()
{
	return {
		filterX.getTau(),
		filterY.getTau(),
		filterZ.getTau(),
	};
}

vec3f LowPassFilter3f::process(vec3f value)
{
	return { filterX.process(value.x), filterY.process(value.y), filterZ.process(value.z) };
}

vec3f LowPassFilter3f::process(vec3f value, float dt)
{
	return { filterX.process(value.x, dt), filterY.process(value.y, dt), filterZ.process(value.z, dt) };
}



LowPassFilter4f::LowPassFilter4f() : filterX(), filterY(), filterZ(), filterR() {}

LowPassFilter4f::LowPassFilter4f(float tau) : filterX(tau), filterY(tau), filterZ(tau), filterR(tau) {}

LowPassFilter4f::LowPassFilter4f(vec4f tau) : filterX(tau.x), filterY(tau.y), filterZ(tau.z), filterR(tau.r) {}

void LowPassFilter4f::setTau(float tau)
{
	filterX.setTau(tau);
	filterY.setTau(tau);
	filterZ.setTau(tau);
	filterR.setTau(tau);
}

void LowPassFilter4f::setTau(vec4f tau)
{
	filterX.setTau(tau.x);
	filterY.setTau(tau.y);
	filterZ.setTau(tau.z);
	filterR.setTau(tau.r);
}

vec4f LowPassFilter4f::getTau()
{
	return {
		filterX.getTau(),
		filterY.getTau(),
		filterZ.getTau(),
		filterR.getTau(),
	};
}

vec4f LowPassFilter4f::process(vec4f value)
{
	return { filterX.process(value.x), filterY.process(value.y), filterZ.process(value.z), filterR.process(value.r) };
}

vec4f LowPassFilter4f::process(vec4f value, float dt)
{
	return { filterX.process(value.x, dt), filterY.process(value.y, dt), filterZ.process(value.z, dt), filterR.process(value.r, dt) };
}
