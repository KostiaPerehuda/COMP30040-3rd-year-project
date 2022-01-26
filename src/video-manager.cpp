/*
 * video-manager.cpp
 *
 *  Created on: Nov 21, 2021
 *      Author: kostia
 */

#include "video-manager.h"
#include "ardrone/ardrone.h"

VideoManager::VideoManager(ARDrone* drone) : shouldRun_(true), drone_(drone) {
}

void VideoManager::updateListeners(cv::Mat& image) {
//	std::vector<std::thread> threads;

	m_.lock();
	for (std::size_t i = 0; i < imageListeners_.size(); ++i) {
		imageListeners_[i]->onImageReceived(image);

//		threads.push_back(std::thread(&ImageListenerInterface::onImageReceived, imageListeners_[i], image));
	}
	m_.unlock();

//	for (std::size_t i = 0; i < threads.size(); ++i) {
//		threads[i].join();
//	}
//
//	threads.clear();
}

void VideoManager::addImageListener(ImageListenerInterface* listener) {
	m_.lock();
	imageListeners_.push_back(listener);
	m_.unlock();
}

void VideoManager::run() {
	while (shouldRun_) {
		if (drone_->willGetNewImage()) {
			cv::Mat image = drone_->getImage();
			updateListeners(image);
		}
	}
}

void VideoManager::stop() {
	shouldRun_ = false;
}

std::thread VideoManager::spawn() {
	return std::thread(&VideoManager::run, this);
}
