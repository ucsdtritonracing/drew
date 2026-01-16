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
		uint16_t app1;
		uint16_t app2;
	};

	struct BrakePressures {
		uint16_t front;
		uint16_t rear;
	};

	VehicleState();

	const WheelSpeeds getWheelSpeeds() const {
		return wheelSpeeds;
	}

	const AcceleratorPedalPositions getAcceleratorPedalPositions() const {
		return acceleratorPedalPositions;
	}

	const BrakePressures getBrakePressures() const {
		return brakePressures;
	}

	float getSteeringAngleDegrees() const {
		return steeringAngleDegrees;
	}

	bool getReadyToDriveButtonPressed() const {
		return readyToDriveButtonPressed;
	}

	bool getShutdownCircuitClosed() const {
		return shutdownCircuitClosed;
	}

    bool getReadyToDrive() const {
    	return readyToDrive;
    }

    void setWheelSpeedFL(float wheelSpeed) {
    	wheelSpeeds.frontLeft = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
    }

    void setWheelSpeedFR(float wheelSpeed) {
    	wheelSpeeds.frontRight = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
    }

    void setWheelSpeedRL(float wheelSpeed) {
    	wheelSpeeds.rearLeft = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
    }

    void setWheelSpeedRR(float wheelSpeed) {
    	wheelSpeeds.rearRight = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
    }

    void setAcceleratorPedalPositions(uint16_t app1, uint16_t app2) {
    	acceleratorPedalPositions.app1 = app1;
    	acceleratorPedalPositions.app2 = app2;
    }

    void setBrakePressures(uint16_t front, uint16_t rear) {
    	brakePressures.front = front;
    	brakePressures.rear = rear;
    }

	void setSteeringAngleDegrees(float degrees) {
		steeringAngleDegrees = std::isfinite(degrees) ? degrees : 0;;
	}

	void setReadyToDriveButtonPressed(bool status) {
		readyToDriveButtonPressed = status;
	}

	void setShutdownCircuitClosed(bool status) {
		shutdownCircuitClosed = status;
	}

	void setReadyToDrive(bool status) {
    	readyToDrive = status;
    }

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
