/*
 * pid-controller.cpp
 *
 *  Created on: Feb 13, 2022
 *      Author: kostia
 *	Inspired by https://github.com/pms67/PID
 */

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
