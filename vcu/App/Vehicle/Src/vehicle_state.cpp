#include "vehicle_state.hpp"


namespace vehicle {

VehicleState::VehicleState() {
	wheelSpeeds 				= {0, 0, 0, 0};
	acceleratorPedalPositions 	= {0, 0};
	brakePressures 				= {0, 0};

	steeringAngleDegrees 		= 0;

	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;

	readyToDrive 				= false;
}

const VehicleState::WheelSpeeds VehicleState::getWheelSpeeds() const {
	return wheelSpeeds;
}

const VehicleState::AcceleratorPedalPositions VehicleState::getAcceleratorPedalPositions() const {
	return acceleratorPedalPositions;
}

const VehicleState::BrakePressures VehicleState::getBrakePressures() const {
	return brakePressures;
}

float VehicleState::getSteeringAngleDegrees() const {
	return steeringAngleDegrees;
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

void VehicleState::setAcceleratorPedalPositions(uint16_t app1, uint16_t app2) {
	acceleratorPedalPositions.app1 = app1;
	acceleratorPedalPositions.app2 = app2;
}

void VehicleState::setBrakePressures(uint16_t front, uint16_t rear) {
	brakePressures.front = front;
	brakePressures.rear = rear;
}

void VehicleState::setSteeringAngleDegrees(float degrees) {
	steeringAngleDegrees = std::isfinite(degrees) ? degrees : 0;
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
