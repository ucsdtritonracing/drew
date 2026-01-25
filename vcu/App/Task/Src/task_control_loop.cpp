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
		torqueScalar = 0.0f;

		app1Fault.update(!vehicle::vehicleState.getAcceleratorPedalPositions().app1Valid);
//		app2Fault.update(!vehicle::vehicleState.getAcceleratorPedalPositions().app2Valid);
//		frontBSEFault.update(!vehicle::vehicleState.getBrakePressures().frontValid);
//		rearBSEFault.update(!vehicle::vehicleState.getBrakePressures().rearValid);
//		appsPlausibilityFault.update(!torque::isAPPSPlausible(vehicle::vehicleState.getAcceleratorPedalPositions()));
//		if(!torque::isAPPSBrakePedalPlausible(vehicle::vehicleState.getAcceleratorPedalPositions(),
//									vehicle::vehicleState.getBrakePressures()) ||
//									appsBrakePedalPlausibilityFaulted) {
//			updateAPPSBrakePedalPlausibility();
//		}

		requestZeroTorque = app1Fault.torqueInhibited() || app2Fault.torqueInhibited() ||
							frontBSEFault.torqueInhibited() || rearBSEFault.torqueInhibited() ||
							appsPlausibilityFault.torqueInhibited() || appsBrakePedalPlausibilityFaulted;
		torqueScalar = requestZeroTorque ? 0 : torque::computeDriverTorqueRequest(vehicle::vehicleState.getAcceleratorPedalPositions());

		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		float requestedTorque = 0.1f * std::lroundf(10 * std::min(torqueScalar * torque::MAX_TORQUE_LIMIT, 100000 + vehicle::inverter.getTorqueCapability()));
		vehicle::inverter.sendCommandMessage(requestedTorque, true);

	} else {
		vehicle::inverter.sendCommandMessage(0, false);
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
