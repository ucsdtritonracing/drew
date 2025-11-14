#include "task_can_bus.hpp"
#include "task.hpp"
#include "cmsis_os.h"

namespace tasks {

void CANBusTask::loop() {
	drivers::CAN::Message message;
	uint8_t messagePriority;
	if (osMessageQueueGet(queue, &message, &messagePriority, osWaitForever) == osOK) {
		CANBus->processMessage(&message);
	}
}

} // namespace tasks
