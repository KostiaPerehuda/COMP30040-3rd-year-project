#include "pid-controller.h"

PidController::PidController(float Kp, float Ki, float Kd, float maxOutput, float minOutput, float tau)
	: Kp(Kp), Ki(Ki), Kd(Kd), maxOutput(maxOutput), minOutput(minOutput), tau(tau),
	proportional(0), integral(0), differential(0), previousError(0), previousMeasurement(0) {}

float PidController::update(float setpoint, float measurement, float dt)
{
	// Error
	float error = setpoint - measurement;


	// Proportional component
	proportional = Kp * error;


	// Integral component with anti-wind-up via clamping
	integral += Ki * dt * (error + previousError) / 2.0f;

	float maxIntegral = (proportional > maxOutput) ? (maxOutput - proportional) : 0.0f;
	float minIntegral = (proportional > minOutput) ? (minOutput - proportional) : 0.0f;

	if      (integral > maxIntegral)  integral = maxIntegral;
	else if (integral < minIntegral)  integral = minIntegral;


	// Differential component with derivative on measurement to avoid kick on setpoint change
	// and with Low pass filter
	float difference = -Kd * (measurement - previousMeasurement);
	differential = (2.0f * difference + (2.0f * tau - dt) * differential) / (2.0f * tau + dt);


	// Store error and measurement for future use
	previousError = error;
	previousMeasurement = measurement;


	// Compute output and clamp if needed
	float output = proportional + integral + differential;

	if (output > maxOutput) return maxOutput;
	if (output < minOutput) return minOutput;
	return output;
}

float PidController::getKp()
{
	return Kp;
}

float PidController::getKi()
{
	return Ki;
}

float PidController::getKd()
{
	return Kd;
}

float PidController::getTau()
{
	return tau;
}

void PidController::setKp(float KpIn)
{
	Kp = KpIn;
}

void PidController::setKi(float KiIn)
{
	Ki = KiIn;
}

void PidController::setKd(float KdIn)
{
	Kd = KdIn;
}

void PidController::setTau(float tauIn)
{
	tau = tauIn;
}

void PidController::set(float KpIn, float KiIn, float KdIn, float tauIn)
{
	Kp = KpIn;
	Ki = KiIn;
	Kd = KdIn;
	tau = tauIn;
}

void PidController::reset()
{
	proportional = 0.0f;
	integral     = 0.0f;
	differential = 0.0f;

	previousError       = 0.0f;
	previousMeasurement = 0.0f;
}
