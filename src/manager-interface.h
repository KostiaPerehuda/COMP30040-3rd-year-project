/*
 * manager-interface.h
 *
 *  Created on: Jan 30, 2022
 *      Author: kostia
 */

#ifndef MANAGER_INTERFACE_H_
#define MANAGER_INTERFACE_H_


#include <thread>

class ManagerInterface
{
private:
	bool m_isRunning = false;
	bool m_shouldStop = false;
	void run(int deltaMillis);
public:
	virtual bool poll() = 0;
	void stop();
	std::thread spawn(int deltaMillis = 0);

	virtual ~ManagerInterface() = default;
};


#endif /* MANAGER_INTERFACE_H_ */