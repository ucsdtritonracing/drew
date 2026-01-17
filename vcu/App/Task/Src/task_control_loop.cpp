#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "drivers.hpp"
#include "torque.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.getReadyToDrive()) {
		float torqueScalar = 0.0f;

		if (torque::isAPPSPlausible(vehicle::vehicleState.getAcceleratorPedalPositions())) {
			torqueScalar = torque::computeDriverTorqueRequest(
					vehicle::vehicleState.getAcceleratorPedalPositions(),
					vehicle::inverter.getMotorRPM()
			);
		}


		if (!std::isfinite(torqueScalar)) {
			torqueScalar = 0.0f;
		}

		torqueScalar = std::clamp(torqueScalar, 0.0f, 1.0f);
		// TODO: vehicle::inverter.sendCommandMessage(torqueScalar * limit?, inverterEnabled = true);
	} else {
		// TODO: send torque request 0 Nm (or inverterEnabled = false...?)
	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
