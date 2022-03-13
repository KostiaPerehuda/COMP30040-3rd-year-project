/*
 * pid-controller.h
 *
 *  Created on: Feb 13, 2022
 *      Author: kostia
 *	Inspired by https://github.com/pms67/PID
 */

#ifndef PID_H_
#define PID_H_


class PidController
{
private:
	// Controller coefficients
	float Kp, Ki, Kd;

	// Controller components
	float proportional, integral, differential;

	// Output limits
	float minOutput, maxOutput;
	
	// Previous values
	float previousError, previousMeasurement;

	// Derivative low-pass filter time constant
	float tau;

public:
	PidController(float Kp, float Ki, float Kd, float maxOutput, float minOutput, float tau);

	float update(float setpoint, float measurement, float dt);
};


#endif /* PID_H_ */