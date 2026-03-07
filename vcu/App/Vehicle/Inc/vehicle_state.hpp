#pragma once

#include "stdint.h"
#include "cmath"
#include "vehicle_state_types.hpp"
#include "cmsis_os2.h"
#include "freertos.h"


namespace vehicle {

class VehicleState {
public:
	VehicleState();

	const Pedals getPedals() const;
	const WheelSpeeds getWheelSpeeds() const;
	const Steering getSteering() const;

	bool getReadyToDriveButtonPressed() const;
	bool getShutdownCircuitClosed() const;
    bool getReadyToDrive() const;

    void setPedals(Pedals pedals);
	void setSteeringAngleDegrees(Steering steering);
    void setWheelSpeedFL(float wheelSpeed);
    void setWheelSpeedFR(float wheelSpeed);
    void setWheelSpeedRL(float wheelSpeed);
    void setWheelSpeedRR(float wheelSpeed);
	void setReadyToDriveButtonPressed(bool status);
	void setShutdownCircuitClosed(bool status);
	void setReadyToDrive(bool status);

private:
	// High sample rate sensors
	Pedals pedals;
	WheelSpeeds wheelSpeeds;
    Steering steering;

    // Polled signals
	bool readyToDriveButtonPressed;
	bool shutdownCircuitClosed;

	// Abstract state
    bool readyToDrive;

    // Synchronization
	osMutexId_t pedalsMutex;
	StaticSemaphore_t pedalsMutexBuffer;
};

extern VehicleState vehicleState;

} // namespace vehicle
