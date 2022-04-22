#ifndef VIDEO_RECORDER_H_
#define VIDEO_RECORDER_H_


#include "opencv2/videoio.hpp"

#include "core/listeners/image-listener-interface.h"

class VideoRecorder : public ImageListenerInterface
{
private:
	cv::VideoWriter writer;
public:
	VideoRecorder();
	~VideoRecorder();

	void onImageReceived(cv::Mat image) override;
};


#endif /* VIDEO_RECORDER_H_ */
