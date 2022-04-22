#include "display-video-listener.h"

#include <opencv2/highgui/highgui.hpp>

DisplayVideoListener::DisplayVideoListener(std::string windowName) : windowName(windowName)
{
	/*cv::namedWindow(windowName);*/
}

DisplayVideoListener::~DisplayVideoListener()
{
	cv::destroyWindow(windowName);
}

void DisplayVideoListener::onImageReceived(cv::Mat image)
{
	cv::imshow(windowName, image);
	cv::waitKey(1);
}
