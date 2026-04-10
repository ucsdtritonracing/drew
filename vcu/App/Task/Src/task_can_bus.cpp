#include "task_can_bus.hpp"
#include "task.hpp"
#include "cmsis_os.h"
#include "main.h"
#include <cstdio>

namespace tasks {

void CANBusTask::init(drivers::can::CANBus& CANBus, osMessageQueueId_t queue) {
	this->CANBus = &CANBus;
	this->queue = queue;
}

void CANBusTask::loop() {
	drivers::can::Message message;
	uint8_t messagePriority;
	if (osMessageQueueGet(queue, &message, &messagePriority, osWaitForever) == osOK) {
		CANBus->processMessage(&message);
	}
}

} // namespace tasks
