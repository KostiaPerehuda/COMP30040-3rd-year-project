/*
 * video-manager.h
 *
 *  Created on: Nov 20, 2021
 *      Author: kostia
 */

#ifndef VIDEO_MANAGER_H_
#define VIDEO_MANAGER_H_


#include <mutex>
#include <vector>
#include "manager-interface.h"
#include "image-listener-interface.h"
#include "ardrone/ardrone.h"

class VideoManager : public ManagerInterface
{
private:
	std::mutex m_mutex;
	ARDrone* m_drone;
	std::vector<ImageListenerInterface*> m_imageListeners;

	void updateListeners(cv::Mat& image);
public:
	VideoManager(ARDrone* drone);

	void addImageListener(ImageListenerInterface* listener);
	bool poll() override;
};

#endif /* VIDEO_MANAGER_H_ */
