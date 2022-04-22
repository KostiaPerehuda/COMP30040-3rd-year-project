#ifndef CAMERA_H_
#define CAMERA_H_


#include <opencv2/core.hpp>

class Camera
{
private:
	static Camera Front, Bottom;

	static Camera front();
	static Camera bottom();

public:
	static Camera* getFrontCamera();
	static Camera* getBottomCamera();

private:
	const cv::Mat intrinsicParameters;
	Camera(cv::Mat intrinsicParameters);

public:
	const cv::Mat& getIntrinsicParameters() const;
};


#endif /* CAMERA_H_ */
