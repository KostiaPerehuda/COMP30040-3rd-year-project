/*
 * display-video-listener.h
 *
 *  Created on: Nov 21, 2021
 *      Author: kostia
 */

#ifndef DISPLAY_VIDEO_LISTENER_H_
#define DISPLAY_VIDEO_LISTENER_H_


#include "image-listener-interface.h"

class DisplayVideoListener : public ImageListenerInterface
{
private:
	std::string windowName_;
public:
	DisplayVideoListener(std::string windowName);
	~DisplayVideoListener();

	void onImageReceived(cv::Mat image);
};


#endif /* DISPLAY_VIDEO_LISTENER_H_ */
