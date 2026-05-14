#include "vehicle/vehicle_state.hpp"
#include "vehicle/types/pedals_types.hpp"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/steering_types.hpp"
#include "vehicle/types/mode_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "cmsis_os.h"
#include <atomic>


namespace vehicle {

VehicleState::VehicleState() {
	readyToDriveButtonPressed 	= false;
	shutdownCircuitClosed 		= false;
	mode 						= IDLE;
}

// Polled
bool VehicleState::getReadyToDriveButtonPressed() const {
	return readyToDriveButtonPressed.load(std::memory_order_relaxed);
}
bool VehicleState::getShutdownCircuitClosed() const {
	return shutdownCircuitClosed.load(std::memory_order_relaxed);
}
void VehicleState::setReadyToDriveButtonPressed(bool status) {
	readyToDriveButtonPressed.store(status, std::memory_order_relaxed);
}
void VehicleState::setShutdownCircuitClosed(bool status) {
	shutdownCircuitClosed.store(status, std::memory_order_relaxed);
}


// Mode
Mode VehicleState::getMode() const {
	return mode.load(std::memory_order_relaxed);
}
void VehicleState::setMode(Mode newMode) {
	mode.store(newMode, std::memory_order_relaxed);
}


// Faults
bool VehicleState::getAPPFault() const {
	return appFault.load(std::memory_order_relaxed);
}
bool VehicleState::getABPPCFault() const {
	return abppcFault.load(std::memory_order_relaxed);
}
void VehicleState::setAPPFault(bool status) {
	appFault.store(status, std::memory_order_relaxed);
}
void VehicleState::setABPPCFault(bool status) {
	abppcFault.store(status, std::memory_order_relaxed);
}


// Pedals
pedals::State VehicleState::getPedals() const {
	return pedals.get();
}
void VehicleState::setPedals(pedals::State state) {
	pedals.update(std::move(state));
}


// PDU
pdu::State VehicleState::getPDUState() const {
	return pdu.get();
}
void VehicleState::setPDUState(pdu::State state) {
	pdu.update(std::move(state));
}


// Wheel Speeds
wheels::State VehicleState::getWheelSpeeds() const {
	return wheelSpeeds.get();
}
void VehicleState::setWheelSpeeds(wheels::State state) {
	wheelSpeeds.update(std::move(state));
}


// Steering
steering::State VehicleState::getSteering() const {
	return steering.get();
}
void VehicleState::setSteering(steering::State state) {
	steering.update(std::move(state));
}


// Inverter
float VehicleState::getInverterTorqueCapability() const {
	return inverterTorqueCapability.get().torqueCapability;
}
void VehicleState::setInverterTemperature1(inverter::Temperature1& state) {
	inverterTemperature1.update(std::move(state));
}
void VehicleState::setInverterTemperature2(inverter::Temperature2& state) {
	inverterTemperature2.update(std::move(state));
}
void VehicleState::setInverterTemperature3(inverter::Temperature3& state) {
	inverterTemperature3.update(std::move(state));
}
void VehicleState::setInverterMotorPosition(inverter::MotorPosition& state) {
	inverterMotorPosition.update(std::move(state));
}
void VehicleState::setInverterTorqueInformation(inverter::TorqueInformation& state) {
	inverterTorqueInformation.update(std::move(state));
}
void VehicleState::setInverterTorqueCapability(inverter::TorqueCapability& state) {
	inverterTorqueCapability.update(std::move(state));
}
void VehicleState::setInverterInternalStates(inverter::InternalStates& state) {
	inverterInternalStates.update(std::move(state));
}
void VehicleState::setInverterFaultFlags(inverter::FaultFlags& state) {
	inverterFaultFlags.update(std::move(state));
}

bool VehicleState::getConfigurationLocked() const {
	return configurationLocked.load(std::memory_order_relaxed);
}

void VehicleState::setConfigurationLocked(bool locked){
	configurationLocked.store(locked, std::memory_order_relaxed);
}


VehicleState vehicleState{};

} // namespace vehicle
