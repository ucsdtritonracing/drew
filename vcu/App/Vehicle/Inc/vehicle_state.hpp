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
		float app2;
		bool app1Valid;
		bool app2Valid;
	};

	struct BrakePressures {
		float front;
		float rear;
		bool frontValid;
		bool rearValid;
	};

	bool readyToDriveButtonPressed;

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
    void setAcceleratorPedalPosition1(float app, bool valid);
    void setAcceleratorPedalPosition2(float app, bool valid);
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
	bool shutdownCircuitClosed;


	// State
    bool readyToDrive;

};

extern VehicleState vehicleState;

} // namespace vehicle
