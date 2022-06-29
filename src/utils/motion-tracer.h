#ifndef MOTION_TRACER_H_
#define MOTION_TRACER_H_


#include <vector>

#include "drone/stabilisation/motion-listener.h"

class MotionTracer : public MotionListener
{
public:
	MotionTracer() = default;
	~MotionTracer();

	bool update(vec4f motion) override;

private:
	//errorVisualizer.draw(error.z, error.x);
	std::vector<vec2f> points;
	float scale;

	void draw();

	void add(vec2f p);

	void rescalePoints();
	void rescalePointsWith(vec2f p);

	float absoulteOf(float value);

	void reset() override;
};


#endif /* MOTION_TRACER_H_ */