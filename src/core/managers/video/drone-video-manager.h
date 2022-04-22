#ifndef DRONE_VIDEO_MANAGER_H_
#define DRONE_VIDEO_MANAGER_H_


#include "ardrone/ardrone.h"

#include "core/managers/video/abstract-video-manager.h"

class DroneVideoManager : public AbstractVideoManager
{
private:
	ARDrone* drone;

public:
	DroneVideoManager(ARDrone* drone);
	virtual ~DroneVideoManager() = default;

public:
	bool poll() override;
};


#endif /* VIDEO_MANAGER_H_ */
