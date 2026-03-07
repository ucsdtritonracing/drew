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
		const vehicle::VehicleState::AcceleratorPedalPositions acceleratorPedalPositions = vehicle::vehicleState.getAcceleratorPedalPositions();
		const vehicle::VehicleState::BrakePressures brakePressures = vehicle::vehicleState.getBrakePressures();

		const float torqueCapability = vehicle::inverter.getTorqueCapability();

		const float app1 = acceleratorPedalPositions.app1;
		const float app2 = acceleratorPedalPositions.app2;
		const float bsef = brakePressures.front;
		const float bser = brakePressures.rear;
		const float appCommand = acceleratorPedalPositions.app1;	// value being used for calculations

		const bool app1Valid = acceleratorPedalPositions.app1Valid;
		const bool app2Valid = acceleratorPedalPositions.app2Valid;
		const bool bsefValid = brakePressures.frontValid;
		const bool bserValid = brakePressures.rearValid;

		const bool appsPlausible = torque::isAPPSPlausible(app1, app2);
		const bool appsBrakePedalPlausible = torque::isAPPSBrakePedalPlausible(appsBrakePedalPlausibilityFaulted, appCommand, bsef, bser);

		const uint32_t currentTick = HAL_GetTick();

		app1Fault.update(!app1Valid, currentTick);
		app2Fault.update(!app2Valid, currentTick);
		bsefFault.update(!bsefValid, currentTick);
		bserFault.update(!bserValid, currentTick);
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
