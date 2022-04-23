#ifndef SPEED_CONTROLLER_H_
#define SPEED_CONTROLLER_H_


#include "configurations/configuration.h"

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
	Vec2fVisualizer errorVisualizer, speedVisualizer;

	Configuration::PidConfig* config;
	PidController vx, vy, vz, vr, magnitude;
	Timer timer;
	vec4f speed;
	vec4f error;
	bool additive;
	bool useMagnitude;

	bool enabled;
	bool updated;

	float getMagnitude(vec4f v);
	vec4f project(float magnitude, vec4f v);

	void resetSelf();
public:
	SpeedController(bool additive = false, bool useMagnitude = false);

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
	void onLeftThumb(bool pressed) override;
	void onRightTrigger(float value) override;

private:
	void setKp(float Kp);
	void setKi(float Ki);
	void setKd(float Kd);
	void setTau(float tau);

	void set(float Kp, float Ki, float Kd, float tau);
	void setConfig(Configuration::PidConfig*);
};


#endif /* SPEED_CONTROLLER_H_ */
