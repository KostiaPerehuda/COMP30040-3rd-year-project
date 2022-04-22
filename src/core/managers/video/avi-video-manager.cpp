#include "avi-video-manager.h"

#include <iostream>

AviVideoManager::AviVideoManager(std::string path) : videoCapture(path)
{
	if (!videoCapture.isOpened()) {
		std::string error = "ERROR: Cannot open file '" + path + "'!";
		throw std::runtime_error(error);
	}

	std::cout << "Opened video file: " << path << std::endl;

	fps = static_cast<int>(videoCapture.get(cv::CAP_PROP_FPS));
	totalFrameCount = static_cast<int>(videoCapture.get(cv::CAP_PROP_FRAME_COUNT));

	timer = Timer();
}

AviVideoManager::~AviVideoManager()
{
	//delete[] frameBuffer;
}

int AviVideoManager::defaultDelta()
{
	return -1;
	//return 1000 / fps;
}

int AviVideoManager::getFPS()
{
	return fps;
}

bool AviVideoManager::poll()
{
	if (!timer.addIfHasPassed((float) 1/fps))
		return false;

	cv::Mat image;

	if (!videoCapture.isOpened() || !videoCapture.read(image)) {
		std::cout << "Video is finished!" << std::endl;
		if (AbstractManager* app = getApplicaton()) app->stop(); stop();
		return false;
	}

	updateListeners(image);

	//std::cout << timer.get() + (float) 1 / fps << std::endl;

	//std::cout << frameCount << " " << totalFrameCount << std::endl;

	//if (frameCount >= totalFrameCount) {
	//	stop();
	//	return false;
	//}

	//updateListeners(frameBuffer[frameCount++]);
	return true;
}
