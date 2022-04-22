#ifndef ABSTRACT_MANAGER_H_
#define ABSTRACT_MANAGER_H_


#include <thread>

class AbstractManager
{
private:
	bool isRunning;
	bool shouldStop;

public:
	AbstractManager();
	virtual ~AbstractManager() = default;

public:
	virtual int defaultDelta();

public:
	void run(int deltaMillis = -1);
	void stop();

	std::thread spawn(int deltaMillis = -1);

	virtual bool poll() = 0;
};


#endif /* ABSTRACT_MANAGER_H_ */
