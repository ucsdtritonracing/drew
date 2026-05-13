#include "tasks/can_dispatch.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <cstdio>


namespace rtos::tasks {

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

} // namespace rtos::tasks
