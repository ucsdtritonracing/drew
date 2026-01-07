#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.readyToDrive) {
		float torque; /* = computeDriverTorqueRequest(
				vehicle::vehicleState.acceleratorPedalPositions,
				vehicle::inverter.getMotorRPM());
				*/

		if (!std::isfinite(torque)) {
			torque = 0.0f;
		}

		torque = std::clamp(torque, 0.0f, 1.0f);
		// TODO: vehicle::inverter.sendCommandMessage(torque, inverterEnabled = true);
	} else {
		// TODO: send torque request 0 Nm (or inverterEnabled = false...?)
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
