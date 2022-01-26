/*
 * sparse-optical-flow.h
 *
 *  Created on: Nov 22, 2021
 *      Author: kostia
 */

#ifndef LUCAS_KANADE_OPTICAL_FLOW_H_
#define LUCAS_KANADE_OPTICAL_FLOW_H_


#include "image-listener-interface.h"

class LucasKanadeOpticalFlow : public ImageListenerInterface {
private:
	std::string windowName_;
	bool shouldFindCorners_;
	cv::Mat mask_;
	cv::Mat previousGrayImage_;
	std::vector<cv::Point2f> previousPoints_;
public:
	LucasKanadeOpticalFlow(std::string windowName);
	~LucasKanadeOpticalFlow();

	void onImageReceived(cv::Mat image);
};


#endif /* LUCAS_KANADE_OPTICAL_FLOW_H_ */
