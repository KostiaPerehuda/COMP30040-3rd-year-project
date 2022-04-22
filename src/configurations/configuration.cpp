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

	// PID speed controller
	SpeedController pid(pidConfig.Kp, pidConfig.Ki, pidConfig.Kd, pidConfig.tau);
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

	// wait
	applicationThread.join();
	return 0;
}

int Configuration::droneVideoImageProcessing()
{
	// Setup drone
	ARDrone ardrone;

	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
		return -1;
	}

	// setup drone video stream
	DroneVideoManager droneVideoManager(&ardrone);

	// display video
	DisplayAndRecordVideo display("camera");
	droneVideoManager.addImageListener(&display);

	// Create application
	UnionManager application({ &droneVideoManager });
	display.setApplicaton(&application);

	// Launch application
	std::thread applicationThread = application.spawn(5);


	char key;
	while (1) {
		std::cin >> key;
		if (key == 'q') {
			application.stop();
			break;
		}
		else if (key == 'c') {
			ardrone.setCamera(1);
		}
	}


	// wait
	applicationThread.join();
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

	// PID speed controller
	SpeedController pid(pidConfig.Kp, pidConfig.Ki, pidConfig.Kd, pidConfig.tau);
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
	std::thread applicationThread = application.spawn(5);

	// wait
	applicationThread.join();

	return 0;
}

Configuration::PidConfig Configuration::pidConfig = { 0.7f, 0.2f, 0.1f, 0.3f };
