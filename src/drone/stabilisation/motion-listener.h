#ifndef MOTION_LISTENER_H_
#define MOTION_LISTENER_H_


#include "utils/vector.h"

class MotionListener
{
public:
	virtual bool update(vec4f v) = 0;
	virtual void reset();

	virtual ~MotionListener() = default;
};


#endif /* MOTION_LISTENER_H_ */
