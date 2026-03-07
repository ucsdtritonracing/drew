#include "vehicle_state.hpp"
#include "cmsis_os2.h"
#include "atomic"


namespace vehicle {

VehicleState::VehicleState() {
	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;

	readyToDrive 				= false;
}


const Pedals VehicleState::getPedals() const {
	return pedals.get();
}

const WheelSpeeds VehicleState::getWheelSpeeds() const {
	return wheelSpeeds;
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

bool VehicleState::getReadyToDrive() const {
	return readyToDrive.load(std::memory_order_relaxed);
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

void VehicleState::setReadyToDrive(bool status) {
	readyToDrive.store(status, std::memory_order_relaxed);
}

VehicleState vehicleState{};

} // namespace vehicle
