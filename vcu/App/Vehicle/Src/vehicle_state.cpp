#include "vehicle_state.hpp"
#include "vehicle_state_types.hpp"
#include "cmsis_os2.h"

namespace vehicle {

VehicleState::VehicleState() {
	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;
	mode 						= IDLE;
}

const Pedals VehicleState::getPedals() const {
	return pedals;
}

const WheelSpeeds VehicleState::getWheelSpeeds() const {
	return wheelSpeeds;
}

const Steering VehicleState::getSteering() const {
	return steering;
}

bool VehicleState::getReadyToDriveButtonPressed() const {
	return readyToDriveButtonPressed;
}

bool VehicleState::getShutdownCircuitClosed() const {
	return shutdownCircuitClosed;
}

Mode VehicleState::getMode() const {
	return mode;
}

void VehicleState::setWheelSpeedFL(float wheelSpeed) {
	wheelSpeeds.frontLeft = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
}

void VehicleState::setWheelSpeedFR(float wheelSpeed) {
	wheelSpeeds.frontRight = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
}

void VehicleState::setWheelSpeedRL(float wheelSpeed) {
	wheelSpeeds.rearLeft = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
}

void VehicleState::setWheelSpeedRR(float wheelSpeed) {
	wheelSpeeds.rearRight = std::isfinite(wheelSpeed) ? wheelSpeed : 0;
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

	this->pedals = std::move(pedals);
}

void VehicleState::setSteeringAngleDegrees(Steering steering) {
	this->steering = std::move(steering);
}

void VehicleState::setReadyToDriveButtonPressed(bool status) {
	readyToDriveButtonPressed = status;
}

void VehicleState::setShutdownCircuitClosed(bool status) {
	shutdownCircuitClosed = status;
}

void VehicleState::setMode(Mode newMode) {
	mode = newMode;
}

VehicleState vehicleState{};

} // namespace vehicle
