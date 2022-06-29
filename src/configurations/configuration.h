#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include <string>

namespace Configuration
{
	struct PidConfig
	{
		float Kp, Ki, Kd, tau;
		float KpSensitivity, KiSensitivity, KdSensitivity, tauSensitivity;
	};

	extern PidConfig pidConfig;
	extern PidConfig pidConfigAdditive;


	int minimalDroneFlight();
	int droneFlight();
	int filesystemVideoImageProcessing(std::string name);

	int filesystemVideoMotionEstimatorTest(std::string name);
};

#endif /* CONFIGURATION_H_ */
