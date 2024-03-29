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

	// Integrator limits
	float minIntegral, maxIntegral;

	// Output limits
	float minOutput, maxOutput;
	
	// Previous values
	float previousError, previousMeasurement;

	// Derivative low-pass filter time constant
	float tau;

public:
	PidController(float Kp, float Ki, float Kd, float maxOutput, float minOutput, float tau, float integralClampingRangeProportion);

	PidController(float Kp, float Ki, float Kd, float maxOutput, float minOutput, float tau, float maxIntegral, float minIntegral);

	float update(float setpoint, float measurement, float dt);

	float getKp();
	float getKi();
	float getKd();
	float getTau();

	void setKp(float Kp);
	void setKi(float Ki);
	void setKd(float Kd);
	void setTau(float tau);

	void set(float Kp, float Ki, float Kd, float tau);

	void reset();

private:
	void setIntegralClampingRange(float integralClampingRangeProportion);
};


#endif /* PID_H_ */
