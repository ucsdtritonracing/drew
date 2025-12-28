#pragma once

#include "stdint.h"


namespace vehicle {

class VehicleState {
public:
	VehicleState();

	// High sample rate sensors
	struct WheelSpeed {
		float frontLeft;
		float frontRight;
		float rearLeft;
		float rearRight;
	} wheelSpeeds;

	struct AcceleratorPedalPositions {
		uint16_t app1;
		uint16_t app2;
	} acceleratorPedalPositions;

	struct BrakePressures {
		uint16_t front;
		uint16_t rear;
	} brakePressures;

    float steeringAngleDegrees;


    // Polled signals
	bool bmsFault;
	bool imdFault;
	bool readyToDriveButtonPressed;
	bool shutdownCircuitClosed;


	// State
    bool readyToDrive;

};

extern VehicleState vehicleState;

} // namespace vehicle
