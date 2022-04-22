#include "lockable-object.h"

LockableObject::LockableObject() : mutex(), locked(false) {}

void LockableObject::lock()
{
	mutex.lock();
	locked = true;
}

void LockableObject::unlock()
{
	if (locked) {
		mutex.unlock();
		locked = false;
	}
}

bool LockableObject::tryLock()
{
	if (locked) return false;

	lock(); return true;
}

bool LockableObject::isLocked()
{
	return locked;
}
