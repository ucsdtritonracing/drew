#include "tasks/wheels.hpp"
#include "vehicle/drivers.hpp"
#include "cmsis_os.h"


namespace tasks {

void WheelsTask::loop() {
	vehicle::wheelsDriver.updateSpeeds();
	osDelay(INTERVAL_MS);
}

} // namespace tasks
