#include "tasks/can_dispatch.hpp"
#include "generics/task.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <cstdio>


namespace tasks {

void CANDispatchTask::init(drivers::can::CANBus& CANBus, osMessageQueueId_t queue) {
	this->CANBus = &CANBus;
	this->queue = queue;
}

void CANDispatchTask::loop() {
	drivers::can::Message message;
	uint8_t messagePriority;
	if (osMessageQueueGet(queue, &message, &messagePriority, osWaitForever) == osOK) {
		CANBus->processMessage(&message);
	}
}

} // namespace tasks
