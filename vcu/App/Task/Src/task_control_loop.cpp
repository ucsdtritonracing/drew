#include "task.hpp"
#include "task_control_loop.hpp"
#include "vehicle_state.hpp"
#include "cmsis_os.h"

namespace tasks {

void ControlLoopTask::loop() {
	if (vehicle::vehicleState.readyToDrive) {

	}

	osDelay(CONTROL_LOOP_PERIOD_MS);
}

} // namespace tasks
