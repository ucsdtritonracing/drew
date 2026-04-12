#pragma once

#include "generics/snapshot.hpp"
#include "vehicle/types/vehicle_state_types.hpp"
#include <stdint.h>
#include <cmath>
#include <atomic>


namespace vehicle {

class VehicleState {
public:
	VehicleState();

	const Pedals getPedals() const;
	const WheelSpeeds getWheelSpeeds() const;
	const Steering getSteering() const;

	bool getReadyToDriveButtonPressed() const;
	bool getShutdownCircuitClosed() const;
    Mode getMode() const;

    void setPedals(Pedals pedals);
	void setSteeringAngleDegrees(Steering steering);
    void setWheelSpeeds(WheelSpeeds wheelSpeeds);
	void setReadyToDriveButtonPressed(bool status);
	void setShutdownCircuitClosed(bool status);
	void setMode(Mode newMode);

private:
	// High sample rate sensors
	Snapshot<Pedals> pedals;
	Snapshot<WheelSpeeds> wheelSpeeds;
	Snapshot<Steering> steering;


    // Polled signals
	std::atomic<bool> readyToDriveButtonPressed;
	std::atomic<bool> shutdownCircuitClosed;


	// Abstract State
    std::atomic<Mode> mode;

};

extern VehicleState vehicleState;

} // namespace vehicle
