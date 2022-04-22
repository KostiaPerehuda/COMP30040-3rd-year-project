#ifndef TIMER_H_
#define TIMER_H_


#include <chrono>

class Timer
{
private:
	float deltaTime;
	std::chrono::milliseconds oldTime, currentTime;

	static std::chrono::milliseconds getTime();
public:
	Timer();

	float loop();
	float get();

	void add(float time);
	bool hasPassed(float time);
	bool addIfHasPassed(float time);
};


#endif /* TIMER_H_ */
