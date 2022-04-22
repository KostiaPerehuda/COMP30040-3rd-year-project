#ifndef VEC2F_VISUALIZER_H_
#define VEC2F_VISUALIZER_H_


#include <string>
#include <opencv2/core.hpp>

class Vec2fVisualizer
{
private:
	std::string name;

	float minX, maxX, minY, maxY;

	bool batch;

	cv::Mat image;

public:
	Vec2fVisualizer(std::string name);
	Vec2fVisualizer(std::string name, float minX, float maxX, float minY, float maxY);

private:

public:
	void draw(float x, float y);
	void draw(float x, float y, cv::Scalar colour);

	void startBatch();
	void endBatch();
};


#endif /* VEC2F_VISUALIZER_H_ */
