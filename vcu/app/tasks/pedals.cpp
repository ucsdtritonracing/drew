#include "tasks/pedals.hpp"
#include "vehicle/drivers.hpp"
#include "cmsis_os.h"


namespace tasks {

void PedalsTask::loop() {
	osThreadFlagsWait(PEDAL_BUFFER_HALF_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	vehicle::pedalsDriver.processHalfBuffer();

	osThreadFlagsWait(PEDAL_BUFFER_FULL_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	vehicle::pedalsDriver.processFullBuffer();
}

} // namespace tasks
