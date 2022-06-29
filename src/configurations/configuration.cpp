#include "./configuration.h"

#include <iostream>

#include "ardrone/ardrone.h"

#include "core/managers/union-manager.h"
#include "core/managers/gamepad/gamepad-input-manager.h"
#include "core/managers/video/avi-video-manager.h"
#include "core/managers/video/drone-video-manager.h"

#include "drone/drone-commander.h"
#include "drone/stabilisation/motion-estimator.h"
#include "drone/stabilisation/speed-controller.h"

#include "utils/video/display-and-record-video.h"
#include "utils/video/display-video-listener.h"
#include "utils/video/video-recorder.h"
#include "utils/motion-tracer.h"

int Configuration::minimalDroneFlight()
{
	// Setup drone
	ARDrone ardrone;

	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// Setup drone video stream
	DroneVideoManager droneVideoManager(&ardrone);

	// Display video
	DisplayAndRecordVideo display("camera");
	droneVideoManager.addImageListener(&display);

	// Drone commander
	DroneCommander commander(&ardrone);

	// Gamepad
	GamepadInputManager gamepadManager;
	gamepadManager.addGamepadListener(&commander);
	gamepadManager.addGamepadListener(&display);

	// Create application
	UnionManager application({ &droneVideoManager, &gamepadManager, &commander });
	commander.setApplicaton(&application);

	// Launch application
	std::thread applicationThread = application.spawn(5);

	// Wait
	applicationThread.join();

	return 0;
}

int Configuration::droneFlight()
{
	// Setup drone
	ARDrone ardrone;

	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// Setup drone video stream
	DroneVideoManager droneVideoManager(&ardrone);

	// Display video
	DisplayAndRecordVideo display("camera");
	droneVideoManager.addImageListener(&display);

	// Estimate motion
	MotionEstimator estimator;
	droneVideoManager.addImageListener(&estimator);

	// trace motion estimation
	MotionTracer motionTracer;
	estimator.addMotionListener(&motionTracer);

	// PID speed controller
	SpeedController pid(false);
	estimator.addMotionListener(&pid);

	// Drone commander
	DroneCommander commander(&ardrone);
	commander.setStabilizer(&pid);

	// Gamepad
	GamepadInputManager gamepadManager;
	gamepadManager.addGamepadListener(&commander);
	gamepadManager.addGamepadListener(&display);
	gamepadManager.addGamepadListener(&pid);
	
	// Create application
	UnionManager application({&droneVideoManager, &gamepadManager, &commander});
	commander.setApplicaton(&application);

	// Launch application
	std::thread applicationThread = application.spawn(5);

	// Wait
	applicationThread.join();

	// Print final config values
	std::cout << "\nFinal Config Values:\n";
	std::cout << "PID Config: { "
		<< pidConfig.Kp << ", " << pidConfig.Ki << ", "
		<< pidConfig.Kd << ", " << pidConfig.tau << " }\n";
	std::cout << "PID Config Additive: { "
		<< pidConfigAdditive.Kp << ", " << pidConfigAdditive.Ki << ", "
		<< pidConfigAdditive.Kd << ", " << pidConfigAdditive.tau << " }\n";

	return 0;
}

int Configuration::filesystemVideoImageProcessing(std::string name)
{
	// setup fs video stream
	std::string path = "videos/" + name;
	AviVideoManager aviVideoManager(path);

	// print fps
	//std::cout << aviVideoManager.getFPS();

	// display video
	DisplayAndRecordVideo display("camera");
	//display.setApplicaton(&aviVideoManager);
	aviVideoManager.addImageListener(&display);
	
	// estimate motion
	MotionEstimator estimator;
	aviVideoManager.addImageListener(&estimator);

	// trace motion estimation
	MotionTracer motionTracer;
	estimator.addMotionListener(&motionTracer);

	// PID speed controller
	SpeedController pid;
	estimator.addMotionListener(&pid);

	//// launch
	//std::thread videoThread = aviVideoManager.spawn();

	// wait
	//videoThread.join();

	// Gamepad
	GamepadInputManager gamepadManager;
	gamepadManager.addGamepadListener(&pid);

	// Create application
	UnionManager application({ &aviVideoManager, &gamepadManager });
	display.setApplicaton(&application);
	aviVideoManager.setApplicaton(&application);

	// Launch application
	pid.enable();
	application.run(5);

	// Print final config values
	std::cout << "\nFinal Config Values:\n";
	std::cout << "PID Config: { "
		<< pidConfig.Kp << ", " << pidConfig.Ki << ", "
		<< pidConfig.Kd << ", " << pidConfig.tau << " }\n";
	std::cout << "PID Config Additive: { "
		<< pidConfigAdditive.Kp << ", " << pidConfigAdditive.Ki << ", "
		<< pidConfigAdditive.Kd << ", " << pidConfigAdditive.tau << " }\n";

	return 0;
}

int Configuration::filesystemVideoMotionEstimatorTest(std::string name)
{
	{
		// setup fs video stream
		std::string path = "videos/" + name;
		AviVideoManager aviVideoManager(path);

		// print fps
		//std::cout << aviVideoManager.getFPS();

		// display video
		DisplayAndRecordVideo display("camera");
		//display.setApplicaton(&aviVideoManager);
		aviVideoManager.addImageListener(&display);

		// estimate motion
		MotionEstimator estimator;
		aviVideoManager.addImageListener(&estimator);

		// trace motion estimation
		MotionTracer motionTracer;
		estimator.addMotionListener(&motionTracer);

		// Gamepad
		GamepadInputManager gamepadManager;
		//gamepadManager.addGamepadListener(&pid);

		// Create application
		UnionManager application({ &aviVideoManager, &gamepadManager });
		display.setApplicaton(&application);
		aviVideoManager.setApplicaton(&application);

		// Launch application
		//pid.enable();
		application.run(5);
	}

	std::cin.get();

	return 0;
}

// For Instantaneous mode
Configuration::PidConfig Configuration::pidConfig = { 
	//0.600f, 0.100f, 0.150f, 0.30f,
	//0.160f, 0.080f, 0.000f, 0.30f,

	//0.020f, 0.020f, 0.025f, 0.05f,

	0.160f, 0.005f, 0.000f, 0.30f,
	//0.180f, 0.008f, 0.000f, 0.30f,

	0.010f, 0.001f, 0.020f, 0.05f,

};

// For Additive mode
Configuration::PidConfig Configuration::pidConfigAdditive = {
	//0.010f, 0.0010f, 0.020f, 0.30f,
	0.010f, 0.0002f, 0.02f, 0.30f,
	/*0.001f, 0.0000f, 0.000f, 0.30f,*/

	0.001f, 0.0001f, 0.001f, 0.05f,
};
