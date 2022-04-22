#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_


#include <string>

namespace Configuration
{
	struct PidConfig
	{
		float Kp, Ki, Kd, tau;
	};

	extern PidConfig pidConfig;



	int droneVideoImageProcessing();
	int droneFlight();
	int filesystemVideoImageProcessing(std::string name);
};

#endif /* CONFIGURATION_H_ */
