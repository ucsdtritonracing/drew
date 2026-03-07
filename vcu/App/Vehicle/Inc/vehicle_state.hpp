#pragma once

#include "stdint.h"
#include "cmath"


namespace vehicle {

class VehicleState {
public:
	struct WheelSpeeds {
		float frontLeft;
		float frontRight;
		float rearLeft;
		float rearRight;
	};

	struct AcceleratorPedalPositions {
		float app1;
		bool app1Valid;
		float app2;
		bool app2Valid;
	};

	struct BrakePressures {
		float front;
		bool frontValid;
		float rear;
		bool rearValid;
	};

	VehicleState();

	const WheelSpeeds getWheelSpeeds() const;
	const AcceleratorPedalPositions getAcceleratorPedalPositions() const;
	const BrakePressures getBrakePressures() const;
	float getSteeringAngleDegrees() const;
	bool getReadyToDriveButtonPressed() const;
	bool getShutdownCircuitClosed() const;
    bool getReadyToDrive() const;
    void setWheelSpeedFL(float wheelSpeed);
    void setWheelSpeedFR(float wheelSpeed);
    void setWheelSpeedRL(float wheelSpeed);
    void setWheelSpeedRR(float wheelSpeed);
    void setAcceleratorPedalPositions(float app1, float app2);
    void setBrakePressures(float front, float rear);
	void setSteeringAngleDegrees(float degrees);
	void setReadyToDriveButtonPressed(bool status);
	void setShutdownCircuitClosed(bool status);
	void setReadyToDrive(bool status);

private:
	// High sample rate sensors
	WheelSpeeds wheelSpeeds;

	AcceleratorPedalPositions acceleratorPedalPositions;
	BrakePressures brakePressures;

    float steeringAngleDegrees;


    // Polled signals
	bool readyToDriveButtonPressed;
	bool shutdownCircuitClosed;


	// State
    bool readyToDrive;

};

extern VehicleState vehicleState;

} // namespace vehicle
