#include "motion-estimator.h"

#include <iostream>

//#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
//#include <opencv2/videoio.hpp>
#include <opencv2/video.hpp>
#include <opencv2/calib3d.hpp>

MotionEstimator::MotionEstimator()
{
	state = State::FIRST_IMAGE;
	camera = Camera::getBottomCamera();

	imageSize = cv::Size(0, 0);

	previousImage = cv::Mat();
	currentImage  = cv::Mat();

	previousFeatures = std::vector<cv::Point2f>();
	currentFeatures  = std::vector<cv::Point2f>();

	filter = LowPassFilter2f(0.2f);
	timer = Timer();
}

MotionEstimator::~MotionEstimator()
{
}

void MotionEstimator::onImageReceived(cv::Mat image)
{
	colourImage = image.clone();
	cv::cvtColor(image, currentImage, cv::COLOR_BGR2GRAY);

	if (colourImage.size() != imageSize) {
		state = State::FIRST_IMAGE;
	}

	switch (state)
	{
	case State::FIRST_IMAGE:
		onFirstImage(); break;
	case State::SECOND_IMAGE:
		onSecondImage(); break;
	case State::DEFAULT:
		onDefaultImage(); break;
	default:
		break;
	}

	previousImage = currentImage.clone();
	previousFeatures = currentFeatures;

	timer.loop();
}

void MotionEstimator::onFirstImage()
{
	imageSize = currentImage.size();

	findFeatures(currentImage, currentFeatures);
	drawFeatures(currentImage, currentFeatures);

	resetListeners();

	state = State::SECOND_IMAGE;
}

void MotionEstimator::onSecondImage()
{
	if (previousFeatures.size() > 0) {
		lucasKanadeOpticalFlow();

		cv::Point2f displacement = averageDisplacement(previousFeatures, currentFeatures);
		vec2f dst = filter.process({ displacement.x, displacement.y }, timer.get());
		displacement = cv::Point2f(dst.x, dst.y) / 50;

		// update listeners
		updateListeners({ -displacement.y, 0, -displacement.x, 0 });

		// draw mean optical flow
		Vec2fVisualizer v("Optical flow");
		v.draw(displacement.x, displacement.y, cv::Scalar(0, 0, 255));

		drawFeatures(colourImage, previousFeatures, currentFeatures);

		float mag = cv::sqrt(displacement.x * displacement.x + displacement.y * displacement.y);
		if (mag > maxMag) {
			maxMag = mag;
		}
		//std::cout << displacement << "  " << timer.get() << "  " << maxMag << std::endl;
	}
	else {
		findFeatures(currentImage, currentFeatures);
		drawFeatures(currentImage, currentFeatures);
	}

	if (currentFeatures.size() < 12) {
		findFeatures(currentImage, currentFeatures);
	}
}

void MotionEstimator::onDefaultImage()
{
	//lucasKanadeOpticalFlow();
}

void MotionEstimator::findFeatures(cv::Mat& image, std::vector<cv::Point2f>& points)
{
	// goodFeaturesToTrack(image, output vector,
	//	 maxCorners ( 0 = unlimited), quality level (rejects features vith values < level * max feature value found),
	//   min euclidean distance between features), mask (if not empty => subset of image where features are detected)
	//   derivative block size, to use Harris, free Harris param)
	//cv::goodFeaturesToTrack(previousGrayImage_, previousPoints_, 100, 0.3, 7, cv::Mat(), 7, false, 0.04);
	cv::goodFeaturesToTrack(image, points, 0, 0.5, 40, cv::Mat(), 7, false, 0.04);
}

void MotionEstimator::drawFeatures(cv::Mat& image, std::vector<cv::Point2f>& points)
{
	cv::Mat circles, display;

	circles = cv::Mat::zeros(image.size(), image.type());

	for (uint i = 0; i < points.size(); i++) {
		cv::circle(circles, points[i], 5, cv::Scalar(255, 255, 255), -1);
	}

	cv::add(circles, image, display);
	cv::imshow("Features", display);
}

void MotionEstimator::drawFeatures(cv::Mat& image, std::vector<cv::Point2f>& oldPoints, std::vector<cv::Point2f>& newPoints)
{
	cv::Mat circles, display;

	circles = cv::Mat::zeros(image.size(), image.type());

	for (uint i = 0; i < oldPoints.size(); i++) {
		cv::circle(circles, oldPoints[i], 5, cv::Scalar(0, 255, 0), -1);
	}

	for (uint i = 0; i < newPoints.size(); i++) {
		cv::circle(circles, newPoints[i], 5, cv::Scalar(255, 255, 255), -1);
	}

	cv::add(circles, image, display);
	cv::imshow("Features", display);
}

cv::Point2f MotionEstimator::averageDisplacement(std::vector<cv::Point2f>& oldPoints, std::vector<cv::Point2f>& newPoints)
{
	cv::Point2f average(0.0f, 0.0f);

	int count = 0;

	for (uint i = 0; i < newPoints.size(); i++) {
		average += (newPoints[i] - oldPoints[i]);
		count++;
	}

	average /= (count ? count : 1);

	return average;
}

void MotionEstimator::lucasKanadeOpticalFlow()
{
	std::vector<cv::Point2f> newFeatures, goodNewFeatures, goodOldFeatures;
	
	// calculate optical flow
	std::vector<uchar> status;
	std::vector<float> err;
	cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::COUNT)+(cv::TermCriteria::EPS), 10, 0.03);
	
	// calcOpticalFlowPyrLK(prevImage, nextImage, prevPoints, nextPoints, array status of each point(1 = found, 0 = not found),
	// 		errors of each point, size of min window detection, height of pyramid (scaling of detection window),
	//		criteria of iterative algorithms(type, iteration count, accuracy))
	cv::calcOpticalFlowPyrLK(previousImage, currentImage, previousFeatures, newFeatures, status, err, cv::Size(9, 9), 4, criteria);
	
	for (uint i = 0; i < previousFeatures.size(); i++) {
		// Select good points
		if (status[i] == 1) {
			goodNewFeatures.push_back(newFeatures[i]);
			goodOldFeatures.push_back(previousFeatures[i]);
		}
	}

	previousFeatures = std::vector<cv::Point2f>(goodOldFeatures);
	currentFeatures = std::vector<cv::Point2f>(goodNewFeatures);
}

void MotionEstimator::findEssentialMatrix()
{
	//cv::Mat R, R2, t;


	/*essentialMatrix = cv::findEssentialMat(previousFeatures, currentFeatures, camera->getIntrinsicParameters());

	cv::recoverPose(essentialMatrix, previousFeatures, currentFeatures, camera->getIntrinsicParameters(), R, t);*/

	//essentialMatrix = cv::findEssentialMat(previousFeatures, currentFeatures, camera->getIntrinsicParameters());

	////cv::recoverPose(essentialMatrix, p1, p2, camera->getIntrinsicParameters(), R, t);
	//if (essentialMatrix.rows == 3 && essentialMatrix.cols == 3) {

	//	cv::decomposeEssentialMat(essentialMatrix, R, R2, t);

	//	std::cout << t << std::endl;

	//	cv::Point3d t_draw(t);


	//	// draw t
	//	vec2fVisualizer.startBatch();
	//	vec2fVisualizer.draw(t.at<double>(0, 0), t.at<double>(0, 1), cv::Scalar(0, 0, 255));
	//	vec2fVisualizer.draw(t.at<double>(0, 1), t.at<double>(0, 2), cv::Scalar(0, 255, 0));
	//	vec2fVisualizer.draw(t.at<double>(0, 2), t.at<double>(0, 0), cv::Scalar(255, 0, 0));
	//	vec2fVisualizer.endBatch();
	//}

	//std::cout << R << "\n" << t << std::endl;

	//std::cout << t.row(0).col(0).at<double>(0,0) << std::endl;


	//self.px_ref, self.px_cur = featureTracking(self.last_frame, self.new_frame, self.px_ref)
	//E, mask = cv2.findEssentialMat(self.px_cur, self.px_ref, focal = self.focal, pp = self.pp, method = cv2.RANSAC, prob = 0.999, threshold = 1.0)
	//_, self.cur_R, self.cur_t, mask = cv2.recoverPose(E, self.px_cur, self.px_ref, focal = self.focal, pp = self.pp)
	//self.frame_stage = STAGE_DEFAULT_FRAME
	//self.px_ref = self.px_cur


	//Mat cv::findEssentialMat(InputArray  	points1,
	//	InputArray  	points2,
	//	InputArray  	cameraMatrix,
	//	int  	method = RANSAC,
	//	double  	prob = 0.999,
	//	double  	threshold = 1.0,
	//	OutputArray  	mask = noArray()
	//)
	//int cv::recoverPose(InputArray  	E,
	//	InputArray  	points1,
	//	InputArray  	points2,
	//	InputArray  	cameraMatrix,
	//	OutputArray  	R,
	//	OutputArray  	t,
	//	InputOutputArray  	mask = noArray()
	//)
}

void MotionEstimator::homography()
{
	//std::vector<cv::Point2f> p1, p2;
	///*for (uint i = 0; i < 10; i++) {
	//	p1.push_back(cv::Point2f(i*5, i*(-5)));
	//}*/
	//p1 = previousFeatures;
	//p2 = currentFeatures;

	///*for (uint i = 0; i < p1.size(); i++) {
	//	p2[i].x = p2[i].x + 10;
	//}*/
	//std::vector<cv::Mat> Rs, ts, normals;
	//cv::Mat H = cv::findHomography(p1, p2);
	//int solutions = cv::decomposeHomographyMat(H, camera->getIntrinsicParameters(), Rs, ts, normals);

	//
	//for (int i = 0; i < solutions; i++)
	//{
	//	cv::Mat rvec;
	//	cv::Rodrigues(Rs[i], rvec);
	//	//Vec2fVisualizer v("t" + );
	//	std::cout << i << "translation: " << ts[i].t() << ", rotation: " << rvec.t() << std::endl;

	//}
	//std::cout << "\n\n";
}

void MotionEstimator::updateListeners(vec4f v)
{
	lock();
	for (std::size_t i = 0; i < listeners.size(); ++i)
		listeners[i]->update(v);
	unlock();
}

void MotionEstimator::resetListeners()
{
	lock();
	for (std::size_t i = 0; i < listeners.size(); ++i)
		listeners[i]->reset();
	unlock();
}

void MotionEstimator::addMotionListener(MotionListener* listener)
{
	lock();
	listeners.push_back(listener);
	unlock();
}
