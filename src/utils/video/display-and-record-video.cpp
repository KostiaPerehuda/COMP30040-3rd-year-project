#include "display-and-record-video.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

DisplayAndRecordVideo::DisplayAndRecordVideo(std::string windowName)
	: windowName(windowName), recorder(nullptr),
	recordingButtonPressed(false), shutdownButtonPressed(false)
{
	setApplicaton(nullptr);
}

DisplayAndRecordVideo::~DisplayAndRecordVideo()
{
	endRecording();
	cv::destroyWindow(windowName);
}

void DisplayAndRecordVideo::startRecording()
{
	if (isRecording()) return;

	lock();
	recorder = new VideoRecorder();
}

void DisplayAndRecordVideo::endRecording()
{
	if (!isRecording()) return;

	delete recorder;
	unlock();
}

bool DisplayAndRecordVideo::isRecording()
{
	return isLocked();
}

void DisplayAndRecordVideo::onImageReceived(cv::Mat image)
{
	if (isRecording()) {
		recorder->onImageReceived(image);

		cv::Mat circle = cv::Mat::zeros(image.size(), image.type());
		cv::circle(circle, cv::Point(15, 15), 12, cv::Scalar(0, 0, 255), -1);
		cv::Mat imageWithCircle; cv::add(image, circle, imageWithCircle);

		cv::imshow(windowName, imageWithCircle);
	}
	else {
		cv::imshow(windowName, image);
	}

	char key = (char) cv::waitKey(1);

	if (key == 'r') recordingButtonPressed = true;
	if (key == 'q') shutdownButtonPressed  = true;

	if (recordingButtonPressed) {
		recordingButtonPressed = false;
		isRecording() ? endRecording() : startRecording();
	}
	if (shutdownButtonPressed) {
		shutdownButtonPressed = false;
		if (AbstractManager* app = getApplicaton()) app->stop();
	}
		
}

void DisplayAndRecordVideo::onLeftShoulder(bool pressed)
{
	if (pressed) return;
	recordingButtonPressed = true;
}

void DisplayAndRecordVideo::onButtonY(bool pressed)
{
	if (pressed) return;
	shutdownButtonPressed = true;
}
