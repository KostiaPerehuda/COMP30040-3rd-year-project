/*
 * video-manager.cpp
 *
 *  Created on: Nov 21, 2021
 *      Author: kostia
 */

#include "video-manager.h"
#include "ardrone/ardrone.h"

VideoManager::VideoManager(ARDrone* drone) : m_drone(drone) {
}

void VideoManager::updateListeners(cv::Mat& image) {
	m_mutex.lock();
	for (std::size_t i = 0; i < m_imageListeners.size(); ++i) {
		m_imageListeners[i]->onImageReceived(image);
	}
	m_mutex.unlock();
}

void VideoManager::addImageListener(ImageListenerInterface* listener) {
	m_mutex.lock();
	m_imageListeners.push_back(listener);
	m_mutex.unlock();
}

bool VideoManager::poll() {
	if (!m_drone->willGetNewImage()) {
		return false;
	}

	cv::Mat image = m_drone->getImage();
	updateListeners(image);
	return true;
}
