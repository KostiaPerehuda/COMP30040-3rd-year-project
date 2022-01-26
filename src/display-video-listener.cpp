/*
 * display-video-listener.cpp
 *
 *  Created on: Nov 21, 2021
 *      Author: kostia
 */

#include <opencv2/highgui/highgui.hpp>
#include "display-video-listener.h"

DisplayVideoListener::DisplayVideoListener(std::string windowName) : windowName_(windowName) {
}

DisplayVideoListener::~DisplayVideoListener() {
}

void DisplayVideoListener::onImageReceived(cv::Mat image) {
	cv::imshow(windowName_, image);
	cv::waitKey(1);
}
