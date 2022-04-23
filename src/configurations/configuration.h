#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include <string>

namespace Configuration
{
	struct PidConfig
	{
		float Kp, Ki, Kd, tau, sensitivity;
	};

	extern PidConfig pidConfig;
	extern PidConfig pidConfigAdditive;



	int droneVideoImageProcessing();
	int droneFlight();
	int filesystemVideoImageProcessing(std::string name);
};

#endif /* CONFIGURATION_H_ */
