/*
 * manager-interface.cpp
 *
 *  Created on: Jan 30, 2022
 *      Author: kostia
 */

#include "manager-interface.h"

#include <chrono>
#include <thread>
#include <iostream>

void ManagerInterface::run(int deltaMillis)
{
	if (deltaMillis) {
		while (!m_shouldStop) {
			poll();
			std::this_thread::sleep_for(std::chrono::milliseconds(deltaMillis));
		}
	}
	else {
		while (!m_shouldStop) {
			poll();
		}
	}
	m_isRunning = false;
	m_shouldStop = false;
}

void ManagerInterface::stop()
{
	m_shouldStop = true;
}

std::thread ManagerInterface::spawn(int deltaMillis)
{
	if (m_isRunning) {
		throw std::runtime_error("Cannot spawn thread! Manager already running!");
	}

	m_isRunning = true;
	return std::thread(&ManagerInterface::run, this, deltaMillis);
}