#include "task.hpp"
#include "task_apps1.hpp"
#include "vehicle_state.hpp"
#include "drivers.hpp"
#include "cmsis_os.h"
#include <cmath>
#include <algorithm>

namespace tasks {

void APPS1Task::loop() {
	osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
	drivers::apps::State state = vehicle::apps1.processHalfBuffer();
	vehicle::vehicleState.setAcceleratorPedalPosition1(state.app, state.valid);

	osThreadFlagsWait(0x01U, osFlagsWaitAny, osWaitForever);
	state = vehicle::apps1.processFullBuffer();
	vehicle::vehicleState.setAcceleratorPedalPosition1(state.app, state.valid);
}

} // namespace tasks
