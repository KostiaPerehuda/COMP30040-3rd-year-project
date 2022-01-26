/*
 * image-listener.h
 *
 *  Created on: Nov 20, 2021
 *      Author: kostia
 */

#ifndef IMAGE_LISTENER_INTERFACE_H_
#define IMAGE_LISTENER_INTERFACE_H_

#include <opencv2/core/core.hpp>

class ImageListenerInterface {
public:
	virtual void onImageReceived(cv::Mat image) = 0;

	virtual ~ImageListenerInterface() = default;
};


#endif /* IMAGE_LISTENER_INTERFACE_H_ */
