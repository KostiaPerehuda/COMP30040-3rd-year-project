#include "drone-video-manager.h"

DroneVideoManager::DroneVideoManager(ARDrone* drone) : drone(drone) {}

bool DroneVideoManager::poll()
{
	if (!drone->willGetNewImage()) {
		return false;
	}

	cv::Mat image = drone->getImage();
	updateListeners(image);
	return true;
}
