#ifndef MOTION_ESTIMATOR_H_
#define MOTION_ESTIMATOR_H_


#include <vector>

#include "configurations/camera.h"

#include "core/listeners/image-listener-interface.h"

#include "drone/stabilisation/motion-listener.h"

#include "utils/lockable-object.h"
#include "utils/timer.h"
#include "utils/vec2f-visualizer.h"
#include "utils/dsp/low-pass-filter.h"

class MotionEstimator : public ImageListenerInterface, private LockableObject
{
private:
	std::vector<MotionListener*> listeners;
	void updateListeners(vec4f v);
	void resetListeners();
public:
	void addMotionListener(MotionListener* listener);

private:
	enum class State
	{
		FIRST_IMAGE, SECOND_IMAGE, DEFAULT,
	};

private:
	State state;
	Camera* camera;

	cv::Size imageSize;
	cv::Mat previousImage, currentImage, colourImage;
	std::vector<cv::Point2f> previousFeatures, currentFeatures;

public:
	MotionEstimator();
	~MotionEstimator();

	void onImageReceived(cv::Mat image) override;

private:
	void onFirstImage();
	void onSecondImage();
	void onDefaultImage();

	void findFeatures(cv::Mat& image, std::vector<cv::Point2f>& points);
	void drawFeatures(cv::Mat& image, std::vector<cv::Point2f>& points);
	void drawFeatures(cv::Mat& image, std::vector<cv::Point2f>& oldPoints, std::vector<cv::Point2f>& newPoints);

	void lucasKanadeOpticalFlow();
	void motionFromOpticalFlow();

	void findEssentialMatrix();
	void homography();

private:
	cv::Point2f averageDisplacement(std::vector<cv::Point2f>& oldPoints, std::vector<cv::Point2f>& newPoints);

	LowPassFilter2f filter;
	Timer timer;

	float maxMag = 0;
};


#endif /* MOTION_ESTIMATOR_H_ */
