#include "tasks/pedals.hpp"
#include "cmsis_os.h"
#include "generics/task.hpp"
#include "vehicle/drivers.hpp"
#include <cmath>
#include <algorithm>


namespace tasks {

void PedalsTask::loop() {
	osThreadFlagsWait(PEDAL_BUFFER_HALF_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	vehicle::pedalsDriver.processHalfBuffer();

	osThreadFlagsWait(PEDAL_BUFFER_FULL_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	vehicle::pedalsDriver.processFullBuffer();
}

} // namespace tasks
