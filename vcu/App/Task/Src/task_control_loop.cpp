#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "drivers.hpp"
#include "torque.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void ControlLoopTask::setup() {
	appsBrakePedalPlausibilityFaulted = false;
}

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.getReadyToDrive()) {
		const vehicle::Pedals pedals = vehicle::vehicleState.getPedals();
		const float torqueCapability = vehicle::inverter.getTorqueCapability();

		const float appCommand = pedals.app1;	// value being used for calculations

		const bool appsPlausible = torque::isAPPSPlausible(pedals.app1, pedals.app2);
		const bool appsBrakePedalPlausible = torque::isAPPSBrakePedalPlausible(appsBrakePedalPlausibilityFaulted, appCommand, pedals.bsef, pedals.bser);

		const uint32_t currentTick = HAL_GetTick();

		app1Fault.update(!pedals.app1Valid, currentTick);
		app2Fault.update(!pedals.app2Valid, currentTick);
		bsefFault.update(!pedals.bsefValid, currentTick);
		bserFault.update(!pedals.bserValid, currentTick);
		appsPlausibilityFault.update(!appsPlausible, currentTick);
		appsBrakePedalPlausibilityFaulted = appsBrakePedalPlausible;

		const bool torqueInhibited = app1Fault.torqueInhibited(currentTick) ||
									 app2Fault.torqueInhibited(currentTick) ||
									 bsefFault.torqueInhibited(currentTick) ||
									 bserFault.torqueInhibited(currentTick) ||
									 appsPlausibilityFault.torqueInhibited(currentTick) ||
									 appsBrakePedalPlausibilityFaulted;

		float torqueScalar = 0.0f;

		if (!torqueInhibited) {
			torqueScalar = torque::computeDriverTorqueRequest(appCommand);
		}

		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		const float driverTorqueRequestNm = torqueScalar * torque::MAX_TORQUE_LIMIT_NM;
		const uint16_t torqueRequest = static_cast<uint16_t>(std::lroundf(std::min(driverTorqueRequestNm, torqueCapability)));
		vehicle::inverter.sendCommandMessage(torqueRequest, true);
	} else {
		vehicle::inverter.sendCommandMessage(0, false);
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
