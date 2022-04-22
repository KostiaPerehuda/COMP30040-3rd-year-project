#ifndef DISPLAY_VIDEO_LISTENER_H_
#define DISPLAY_VIDEO_LISTENER_H_


#include "core/listeners/image-listener-interface.h"

class DisplayVideoListener : public ImageListenerInterface
{
private:
	std::string windowName;

public:
	DisplayVideoListener(std::string windowName);
	~DisplayVideoListener();

	void onImageReceived(cv::Mat image) override;
};


#endif /* DISPLAY_VIDEO_LISTENER_H_ */
