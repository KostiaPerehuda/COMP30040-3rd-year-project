#include "vec2f-visualizer.h"

#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

Vec2fVisualizer::Vec2fVisualizer(std::string name)
	: name(name), minX(-1), maxX(1), minY(-1), maxY(1), batch(false), image() {}

Vec2fVisualizer::Vec2fVisualizer(std::string name, float minX, float maxX, float minY, float maxY)
	: name(name), minX(minX), maxX(maxX), minY(minY), maxY(maxY), batch(false), image() {}

void Vec2fVisualizer::draw(float x, float y)
{
	draw(x, y, cv::Scalar(0, 0, 255));
}

void Vec2fVisualizer::draw(float x, float y, cv::Scalar colour)
{
	if (!batch) image = cv::Mat::zeros(cv::Size(300, 300), CV_8UC4);

	cv::Point2f center(150.0f, 150.0f);
	cv::Point2f displacement(150.0f + 150.0f * x / maxX, 150.0f + 150.0f * y / maxY);
	
	cv::line(image, center, displacement, colour, 2);
	cv::circle(image, displacement, 5, colour, -1);

	if (!batch) cv::imshow(name, image);
}

void Vec2fVisualizer::startBatch()
{
	batch = true;
	image = cv::Mat::zeros(cv::Size(300, 300), CV_8UC4);
}

void Vec2fVisualizer::endBatch()
{
	if (!batch) return;
	cv::imshow(name, image);
	batch = false;
}
