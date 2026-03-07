#include "vehicle_state.hpp"
#include "cmsis_os2.h"


namespace vehicle {

VehicleState::VehicleState() {
	osMutexAttr_t mutexAttr;

	mutexAttr = {
	    "PedalsMutex",
		osMutexRobust,
	    &pedalsMutexBuffer,
	    sizeof(pedalsMutexBuffer)
	};
	pedalsMutex = osMutexNew(&mutexAttr);

	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;

	readyToDrive 				= false;
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

bool VehicleState::getReadyToDrive() const {
	return readyToDrive;
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

void VehicleState::setReadyToDrive(bool status) {
	readyToDrive = status;
}

VehicleState vehicleState{};

} // namespace vehicle
