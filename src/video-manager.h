/*
 * video-manager.h
 *
 *  Created on: Nov 20, 2021
 *      Author: kostia
 */

#ifndef VIDEO_MANAGER_H_
#define VIDEO_MANAGER_H_

#include <thread>
#include <mutex>
#include "image-listener-interface.h"
#include "ardrone/ardrone.h"

class VideoManager {
private:
	std::mutex m_;
	bool shouldRun_;
	ARDrone* drone_;
	std::vector<ImageListenerInterface*> imageListeners_;

	void updateListeners(cv::Mat& image);
public:
	VideoManager(ARDrone* drone);
	void addImageListener(ImageListenerInterface* listener);
	void stop();
	void run();
	std::thread spawn();
};

#endif /* VIDEO_MANAGER_H_ */
