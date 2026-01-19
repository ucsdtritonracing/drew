#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "drivers.hpp"
#include "torque.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void ControlLoopTask::computeFaultInstructions(bool& fault, uint32_t& faultTime) {
	if(fault) {
		if((HAL_GetTick() - faultTime) >= 100) {
			requestZeroTorque =  true;
		}
	}
	else {
		fault = true;
		faultTime = HAL_GetTick();
	}
}

void ControlLoopTask::computeFaultInstructions() {
	if(appsBrakePedalPlausibilityFaulted) {
		if(vehicle::vehicleState.getAcceleratorPedalPositions().app1 < 0.05) {
			appsBrakePedalPlausibilityFaulted = false;
		}
		else {
			requestZeroTorque = true;
		}
	}
	else {
		appsBrakePedalPlausibilityFaulted = true;
		requestZeroTorque = true;
	}
}

void ControlLoopTask::resetFault(bool& fault, uint32_t& faultTime) {
	if(fault) {
		fault = false;
		faultTime = 0;
	}
}

float ControlLoopTask::computeTorqueSecurity() {
	requestZeroTorque = false;
	if(!vehicle::vehicleState.getAcceleratorPedalPositions().app1Valid) {
		computeFaultInstructions(app1Faulted, app1FaultTime);
	}
	else {
		resetFault(app1Faulted, app1FaultTime);
	}
	if (!vehicle::vehicleState.getAcceleratorPedalPositions().app2Valid) {
		computeFaultInstructions(app2Faulted, app2FaultTime);
	}
	else {
		resetFault(app2Faulted, app2FaultTime);
	}
	if(!vehicle::vehicleState.getBrakePressures().frontValid) {
		computeFaultInstructions(bseFrontFaulted, bseFrontFaultTime);
	}
	else {
		resetFault(bseFrontFaulted, bseFrontFaultTime);
	}
	if(!vehicle::vehicleState.getBrakePressures().rearValid) {
		computeFaultInstructions(bseRearFaulted, bseRearFaultTime);
	}
	else {
		resetFault(bseRearFaulted, bseRearFaultTime);
	}
	if(!torque::isAPPSPlausible(vehicle::vehicleState.getAcceleratorPedalPositions())) {
		computeFaultInstructions(appsPlausibilityFaulted, appsPlausibilityFaultTime);
	}
	else {
		resetFault(appsPlausibilityFaulted, appsPlausibilityFaultTime);
	}
	if(!torque::isAPPSBrakePedalPlausible(vehicle::vehicleState.getAcceleratorPedalPositions(),
								vehicle::vehicleState.getBrakePressures()) ||
								appsBrakePedalPlausibilityFaulted) {
		computeFaultInstructions();
	}
	if(requestZeroTorque){
		return 0;
	}
	return torque::computeDriverTorqueRequest(vehicle::vehicleState.getAcceleratorPedalPositions());
}

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.getReadyToDrive()) {
		float torqueScalar = 0.0f;

		torqueScalar = computeTorqueSecurity();

		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		uint16_t requestedTorque = static_cast<uint16_t>(std::lroundf(std::min(torqueScalar * torque::MAX_TORQUE_LIMIT, vehicle::inverter.getTorqueCapability())));
		vehicle::inverter.sendCommandMessage(requestedTorque, true);

	}
	else {
		vehicle::inverter.sendCommandMessage(0, false);
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
