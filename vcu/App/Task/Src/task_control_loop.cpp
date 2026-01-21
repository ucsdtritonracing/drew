#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "drivers.hpp"
#include "torque.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void ControlLoopTask::updateAPPSBrakePedalPlausibility() {
	if(appsBrakePedalPlausibilityFaulted) {
		if(vehicle::vehicleState.getAcceleratorPedalPositions().app1 < APPS_PEDAL_TRAVEL_RESET_THRESHOLD) {
			appsBrakePedalPlausibilityFaulted = false;
		}
	} else {
		appsBrakePedalPlausibilityFaulted = true;
	}
}

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.getReadyToDrive()) {
		float torqueScalar = 0.0f;

		app1Fault.update(!vehicle::vehicleState.getAcceleratorPedalPositions().app1Valid);
		app2Fault.update(!vehicle::vehicleState.getAcceleratorPedalPositions().app2Valid);
		frontBSEFault.update(!vehicle::vehicleState.getBrakePressures().frontValid);
		rearBSEFault.update(!vehicle::vehicleState.getBrakePressures().rearValid);
		appsPlausibilityFault.update(!torque::isAPPSPlausible(vehicle::vehicleState.getAcceleratorPedalPositions()));
		if(!torque::isAPPSBrakePedalPlausible(vehicle::vehicleState.getAcceleratorPedalPositions(),
									vehicle::vehicleState.getBrakePressures()) ||
									appsBrakePedalPlausibilityFaulted) {
			updateAPPSBrakePedalPlausibility();
		}

		requestZeroTorque = app1Fault.torqueInhibited() || app2Fault.torqueInhibited() ||
							frontBSEFault.torqueInhibited() || rearBSEFault.torqueInhibited() ||
							appsPlausibilityFault.torqueInhibited() || appsBrakePedalPlausibilityFaulted;
		torqueScalar = requestZeroTorque ? 0 : torque::computeDriverTorqueRequest(vehicle::vehicleState.getAcceleratorPedalPositions());

		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		uint16_t requestedTorque = static_cast<uint16_t>(std::lroundf(std::min(torqueScalar * torque::MAX_TORQUE_LIMIT, vehicle::inverter.getTorqueCapability())));
		vehicle::inverter.sendCommandMessage(requestedTorque, true);

	} else {
		vehicle::inverter.sendCommandMessage(0, false);
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
