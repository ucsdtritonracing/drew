#include "vehicle/vehicle_state.hpp"
#include "vehicle/types/vehicle_state_types.hpp"
#include "cmsis_os.h"
#include <atomic>


namespace vehicle {

VehicleState::VehicleState() {
	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;
	mode 						= IDLE;
}

const Pedals VehicleState::getPedals() const {
	return pedals.get();
}

const WheelSpeeds VehicleState::getWheelSpeeds() const {
	return wheelSpeeds.get();
}

const Steering VehicleState::getSteering() const {
	return steering.get();
}

bool VehicleState::getReadyToDriveButtonPressed() const {
	return readyToDriveButtonPressed.load(std::memory_order_relaxed);
}

bool VehicleState::getShutdownCircuitClosed() const {
	return shutdownCircuitClosed.load(std::memory_order_relaxed);
}

Mode VehicleState::getMode() const {
	return mode.load(std::memory_order_relaxed);
}

void VehicleState::setWheelSpeeds(WheelSpeeds wheelSpeeds) {
	if (!std::isfinite(wheelSpeeds.frontLeft)) {
		return;
	}

	if (!std::isfinite(wheelSpeeds.frontRight)) {
		return;
	}

	if (!std::isfinite(wheelSpeeds.rearLeft)) {
		return;
	}

	if (!std::isfinite(wheelSpeeds.rearRight)) {
		return;
	}

	this->wheelSpeeds.update(wheelSpeeds);
}


void VehicleState::setPedals(Pedals pedals) {
	if (pedals.app1 < 0 || pedals.app1 > 1 || !std::isfinite(pedals.app1)) {
		return;
	}

	if (pedals.app2 < 0 || pedals.app2 > 1 || !std::isfinite(pedals.app2)) {
		return;
	}

	if (pedals.bsef < 0 || pedals.bsef > 1 || !std::isfinite(pedals.bsef)) {
		return;
	}

	if (pedals.bser < 0 || pedals.bser > 1 || !std::isfinite(pedals.bser)) {
		return;
	}

	this->pedals.update(std::move(pedals));
}

void VehicleState::setSteeringAngleDegrees(Steering steering) {
	this->steering.update(std::move(steering));
}

void VehicleState::setReadyToDriveButtonPressed(bool status) {
	readyToDriveButtonPressed.store(status, std::memory_order_relaxed);
}

void VehicleState::setShutdownCircuitClosed(bool status) {
	shutdownCircuitClosed.store(status, std::memory_order_relaxed);
}

void VehicleState::setMode(Mode newMode) {
	mode.store(newMode, std::memory_order_relaxed);
}

VehicleState vehicleState{};

} // namespace vehicle
