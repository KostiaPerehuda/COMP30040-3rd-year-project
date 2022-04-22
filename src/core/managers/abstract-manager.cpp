#include "abstract-manager.h"

#include <chrono>
#include <stdexcept>

AbstractManager::AbstractManager() : isRunning(false), shouldStop(false) {}

int AbstractManager::defaultDelta()
{
	return 0;
}

void AbstractManager::run(int deltaMillis)
{
	isRunning = true;

	if (deltaMillis < 0) deltaMillis = defaultDelta();

	if (deltaMillis > 0) {
		while (!shouldStop) {
			poll();
			std::this_thread::sleep_for(std::chrono::milliseconds(deltaMillis));
		}
	}
	else {
		while (!shouldStop) {
			poll();
		}
	}

	isRunning  = false;
	shouldStop = false;
}

void AbstractManager::stop()
{
	if (isRunning) shouldStop = true;
}

std::thread AbstractManager::spawn(int deltaMillis)
{
	if (isRunning) {
		throw std::runtime_error("Cannot spawn thread! Manager already running!");
	}

	isRunning = true;
	return std::thread(&AbstractManager::run, this, deltaMillis);
}
