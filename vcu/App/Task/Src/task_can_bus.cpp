#include "task_can_bus.hpp"
#include "task.hpp"
#include "cmsis_os.h"
#include "main.h"
#include <cstdio>

namespace tasks {

CANBusTask::CANBusTask(const drivers::CAN::CANBus& CANBus, osMessageQueueId_t queue)
		: CANBus(CANBus), queue(queue) {}

void CANBusTask::loop() {
	drivers::CAN::Message message;
	uint8_t messagePriority;
	if (osMessageQueueGet(queue, &message, &messagePriority, osWaitForever) == osOK) {
		CANBus.processMessage(&message);
	}
}

} // namespace tasks
