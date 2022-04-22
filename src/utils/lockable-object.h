#ifndef LOCKABLE_CLASS_H_
#define LOCKABLE_CLASS_H_


#include <mutex>

class LockableObject
{
private:
	std::mutex mutex;
	bool locked;

public:
	LockableObject();
	virtual ~LockableObject() = default;

	void lock();
	void unlock();
	bool tryLock();
	bool isLocked();
};


#endif /* LOCKABLE_CLASS_H_ */
