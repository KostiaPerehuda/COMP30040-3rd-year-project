/*
 * OUTDATED FILE
 */

#ifndef LUCAS_KANADE_OPTICAL_FLOW_H_
#define LUCAS_KANADE_OPTICAL_FLOW_H_


#include "image-listener-interface.h"
#include "low-pass-filter.h"
#include "speed-controller.h"
#include "vec2f-visualizer.h"

class LucasKanadeOpticalFlow : public ImageListenerInterface
{
private:
	SpeedController* controller;
	Vec2fVisualizer visualizer;

	std::string windowName_;
	bool shouldFindCorners_;
	cv::Mat mask_;
	cv::Mat previousGrayImage_;
	std::vector<cv::Point2f> previousPoints_;
	LowPassFilter filterX, filterY;


public:
	LucasKanadeOpticalFlow(std::string windowName, SpeedController* controller);
	~LucasKanadeOpticalFlow();

	void onImageReceived(cv::Mat image);
};


#endif /* LUCAS_KANADE_OPTICAL_FLOW_H_ */
