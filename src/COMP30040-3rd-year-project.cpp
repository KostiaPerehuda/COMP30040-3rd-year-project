//============================================================================
// Name        : COMP30040-3rd-year-project.cpp
// Author      : Kostiantyn Perehuda
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <thread>

#include <opencv2/opencv.hpp>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "ardrone/ardrone.h"
#include "video-manager.h"
#include "display-video-listener.h"
#include "lucas-kanade-optical-flow.h"
#include "image-listener-interface.h"


int main() {
	// AR.Drone class
	ARDrone ardrone;

	// Initialize
	if (!ardrone.open()) {
		std::cout << "Failed to initialize." << std::endl;
//			delete ardrone;
		return -1;
	}

	VideoManager manager(&ardrone);

	DisplayVideoListener listener1("camera1");
//	DisplayVideoListener listener2("camera2");
	LucasKanadeOpticalFlow listener3("lk-optical-flow");

	manager.addImageListener(&listener1);
//	manager.addImageListener(&listener2);
	manager.addImageListener(&listener3);

	std::thread t = manager.spawn();

	char key;
	while (1) {
		std::cin >> key;
		if (key == 'q') {
			manager.stop();
			break;
		}
	}

	t.join();
	// See you
	ardrone.close();

//	delete listener1;
//	delete listener2;
//	delete manager;
//	delete ardrone;

	return 0;
}
