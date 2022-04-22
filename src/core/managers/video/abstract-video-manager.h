#ifndef ABSTRACT_VIDEO_MANAGER_H_
#define ABSTRACT_VIDEO_MANAGER_H_


#include <vector>

#include "core/managers/abstract-manager.h"
#include "core/listeners/image-listener-interface.h"

#include "utils/lockable-object.h"

class AbstractVideoManager : public AbstractManager, private LockableObject
{
private:
	std::vector<ImageListenerInterface*> listeners;

public:
	AbstractVideoManager();
	virtual ~AbstractVideoManager() = default;

protected:
	void updateListeners(cv::Mat& image);

public:
	void addImageListener(ImageListenerInterface* listener);
};


#endif /* ABSTRACT_VIDEO_MANAGER_H_ */
