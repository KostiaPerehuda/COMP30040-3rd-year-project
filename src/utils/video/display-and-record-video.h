#ifndef DISPLAY_AND_RECORD_VIDEO_H_
#define DISPLAY_AND_RECORD_VIDEO_H_


#include "core/application-container.h"
#include "core/listeners/gamepad-listener-interface.h"
#include "core/listeners/image-listener-interface.h"

#include "utils/lockable-object.h"
#include "utils/video/video-recorder.h"

class DisplayAndRecordVideo : 
	public ImageListenerInterface, public GamepadListenerAdapter, public ApplicatonContainer, private LockableObject
{
private:
	std::string windowName;
	VideoRecorder* recorder;

	bool recordingButtonPressed;
	bool shutdownButtonPressed;

public:
	DisplayAndRecordVideo(std::string windowName);
	~DisplayAndRecordVideo();

	void startRecording();
	void endRecording();
	bool isRecording();

	void onImageReceived(cv::Mat image) override;
	void onLeftShoulder(bool pressed) override;
	void onButtonY(bool pressed) override;
};


#endif /* DISPLAY_AND_RECORD_VIDEO_H_ */
