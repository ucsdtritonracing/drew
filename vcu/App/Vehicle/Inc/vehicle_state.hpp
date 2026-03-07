#pragma once

#include "stdint.h"
#include "cmath"
#include "atomic"
#include "vehicle_state_types.hpp"
#include "snapshot.hpp"


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
	Snapshot<Pedals> pedals;
	WheelSpeeds wheelSpeeds;
    Snapshot<Steering> steering;

    // Polled signals
	std::atomic<bool> readyToDriveButtonPressed;
	std::atomic<bool> shutdownCircuitClosed;

	// Abstract state
    std::atomic<bool> readyToDrive;
};

extern VehicleState vehicleState;

} // namespace vehicle
