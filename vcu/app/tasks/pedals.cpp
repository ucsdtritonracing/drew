#include "tasks/pedals.hpp"
#include "cmsis_os.h"
#include "generics/task.hpp"
#include "vehicle/drivers.hpp"
#include "vehicle/vehicle_state.hpp"
#include <cmath>
#include <algorithm>


namespace tasks {

void PedalsTask::loop() {
	osThreadFlagsWait(PEDAL_BUFFER_HALF_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	vehicle::Pedals pedals = vehicle::pedals.processHalfBuffer();
	vehicle::vehicleState.setPedals(pedals);

	osThreadFlagsWait(PEDAL_BUFFER_FULL_COMPLETE_FLAG, osFlagsWaitAny, osWaitForever);
	pedals = vehicle::pedals.processFullBuffer();
	vehicle::vehicleState.setPedals(pedals);
}

} // namespace tasks
