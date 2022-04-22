#ifndef DRONE_COMMANDER_H_
#define DRONE_COMMANDER_H_


#include "ardrone/ardrone.h"

#include "core/application-container.h"
#include "core/listeners/gamepad-listener-interface.h"
#include "core/managers/abstract-manager.h"

#include "drone/stabilisation/speed-controller.h"

#include "utils/lockable-object.h"

enum class CommanderState {
	MOTION_REQUESTED,
	STABILIZATION_REQUESTED,
	IDLE,
	ON_GROUND,
};

struct DroneSettings {
	bool cameraMode;
	bool dualRate;
	bool autoStabilization;
};

class DroneCommander : public GamepadListenerAdapter, public AbstractManager, public ApplicatonContainer, private LockableObject
{
private:

	ARDrone* drone;
	DroneSettings settings;

	SpeedController* stabilizer;

	vec4f speed;
	bool isSpeedUpdated;

	vec4f toMove3DFormat(vec4f speed);
	vec4f toTilt3DFormat(vec4f speed, bool dualRate = false);

	bool isSpeedNonZero();

	void takeoff();
	void land();
	void takeoffOrLand();

	void switchCamera();
	void switchRate();

	void enableStabilizer();
	void disableStabilizer();
	void enableOrDisableStabilization();

	void shutdown();
public:
	DroneCommander(ARDrone* drone);
	DroneCommander(ARDrone* drone, SpeedController* stabilizer);

	SpeedController* getStabilizer();
	void setStabilizer(SpeedController* stabilizer);

	void onButtonA(bool pressed) override;
	void onButtonB(bool pressed) override;
	void onButtonX(bool pressed) override;
	void onButtonY(bool pressed) override;
	void onLeftShoulder(bool pressed) override;
	void onRightShoulder(bool pressed) override;

	void onLeftTrigger(float value) override;

	void onLeftThumb(float x, float y) override;
	void onRightThumb(float x, float y) override;

	bool poll() override;
};


#endif /* DRONE_COMMANDER_H_ */
