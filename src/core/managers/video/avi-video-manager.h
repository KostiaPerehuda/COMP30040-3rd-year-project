#ifndef AVI_VIDEO_MANAGER_H_
#define AVI_VIDEO_MANAGER_H_


#include <string>
#include <opencv2/videoio/videoio.hpp>

#include "core/application-container.h"
#include "core/managers/video/abstract-video-manager.h"

#include "utils/timer.h"

class AviVideoManager : public AbstractVideoManager, public ApplicatonContainer
{
private:
	cv::VideoCapture videoCapture;

	Timer timer;

	int fps, totalFrameCount;

public:
	AviVideoManager(std::string path);
	virtual ~AviVideoManager();

public:
	int defaultDelta() override;

public:
	int getFPS();

	bool poll() override;
};


#endif /* AVI_VIDEO_MANAGER_H_ */
