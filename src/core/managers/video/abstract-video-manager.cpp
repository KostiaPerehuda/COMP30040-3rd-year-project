#include "abstract-video-manager.h"

AbstractVideoManager::AbstractVideoManager() {}

void AbstractVideoManager::updateListeners(cv::Mat& image)
{
	lock();
	for (std::size_t i = 0; i < listeners.size(); ++i)
		listeners[i]->onImageReceived(image);
	unlock();
}

void AbstractVideoManager::addImageListener(ImageListenerInterface* listener)
{
	lock();
	listeners.push_back(listener);
	unlock();
}
