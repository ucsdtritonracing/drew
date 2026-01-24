#include "test_task.hpp"
#include "task.hpp"
#include "cmsis_os.h"
#include "main.h"
#include <cstdio>
#include "vehicle_state.hpp"
#include "drivers.hpp"

namespace tasks {

vehicle::VehicleState state;
TestTask::TestTask(uint8_t please) {
	uint8_t work = please;
}

void TestTask::loop() {
	printf("im alive \n");
	counter+=1;
	state.setWheelSpeedFR(static_cast<float>(counter));
	osDelay(250);
}

} // namespace tasks
