#include "motion-tracer.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

MotionTracer::~MotionTracer()
{
	std::cout << points.back().x / scale << " " << " " << points.back().y / scale << "\n";
}

bool MotionTracer::update(vec4f feedback)
{
	add({ feedback.z, feedback.x });
	draw();

	return true;
}



void MotionTracer::draw()
{
	cv::Mat image = cv::Mat::zeros(cv::Size(400, 400), CV_8UC4);

	vec2f center = { 200.0f, 200.0f };
	cv::Point2f displacement;

	for (vec2f p : points) {
		displacement.x = 200.0f + (190.0f * p.x / scale);
		displacement.y = 200.0f + (190.0f * p.y / scale);

		cv::circle(image, displacement, 3, cv::Scalar(0, 0, 255), -1);
	}
	if (!points.empty()) {
		displacement.x = 200.0f + (190.0f * points.back().x / scale);
		displacement.y = 200.0f + (190.0f * points.back().y / scale);

		cv::circle(image, displacement, 8, cv::Scalar(255, 255, 0), -1);

	}

	cv::imshow("Motion Tracer", image);
}

void MotionTracer::add(vec2f point)
{
	if (points.empty()) {
		points.push_back(point);
	}
	else {
		points.push_back({ points.back().x + point.x, points.back().y + point.y });
	}
	
	rescalePointsWith(points.back());
	//rescalePoints();
}

void MotionTracer::rescalePoints()
{
	for (vec2f p : points) {
		//std::cout << p.x << " " << p.y << " => ";
		p = { absoulteOf(p.x), absoulteOf(p.y) };
		//std::cout << p.x << " " << p.y<< "\n";
		if (p.x > scale)
			scale = p.x;
		if (p.y > scale)
			scale = p.y;
	}
}

void MotionTracer::rescalePointsWith(vec2f point)
{
	point = { absoulteOf(point.x), absoulteOf(point.y) };
	if (point.x > scale)
		scale = point.x;
	if (point.y > scale)
		scale = point.y;
}

float MotionTracer::absoulteOf(float value) {
	return value >= 0 ? value : -value;
}

void MotionTracer::reset()
{
	points.clear();
	scale = 0.0f;
	draw();
}
