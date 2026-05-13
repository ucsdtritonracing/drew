#pragma once

#include "rtos/task.hpp"
#include "drivers/can/can_bus.hpp"
#include "cmsis_os.h"


namespace rtos::tasks {

class CANDispatchTask: public Task<CANDispatchTask, osPriorityHigh, 512> {
public:
	void init(drivers::can::CANBus& CANBus, osMessageQueueId_t queue);
	void loop();

private:
	drivers::can::CANBus* CANBus;
	osMessageQueueId_t queue;
};

} // namespace rtos::tasks
