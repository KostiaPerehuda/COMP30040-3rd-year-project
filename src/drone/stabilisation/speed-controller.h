#ifndef SPEED_CONTROLLER_H_
#define SPEED_CONTROLLER_H_


#include "core/listeners/gamepad-listener-interface.h"

#include "drone/stabilisation/motion-listener.h"

#include "utils/dsp/pid-controller.h"
#include "utils/lockable-object.h"
#include "utils/timer.h"
#include "utils/vector.h"
#include "utils/vec2f-visualizer.h"

class SpeedController : public MotionListener, public GamepadListenerAdapter, private LockableObject
{
private:
	Vec2fVisualizer visualizer;

	PidController vx, vy, vz, vr;
	Timer timer;
	vec4f speed;

	bool enabled;
	bool updated;

	void resetSelf();
public:
	SpeedController(float Kp, float Ki, float Kd, float tau);

	bool update(vec4f feedback) override;
	bool isUpdated();

	vec4f getSpeed();

	void enable();
	void disable();
	bool isEnabled();

	void reset() override;

public:
	void onDpadUp(bool pressed) override;
	void onDpadDown(bool pressed) override;
	void onDpadLeft(bool pressed) override;
	void onDpadRight(bool pressed) override;
	void onStart(bool pressed) override;
	void onBack(bool pressed) override;
};


#endif /* SPEED_CONTROLLER_H_ */
