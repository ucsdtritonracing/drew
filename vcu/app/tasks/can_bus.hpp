#pragma once

#include "generics/task.hpp"
#include "drivers/can/can_bus.hpp"
#include "cmsis_os.h"


namespace tasks {

class CANBusTask: public Task<CANBusTask, osPriorityHigh, 512> {
public:
	void init(drivers::can::CANBus& CANBus, osMessageQueueId_t queue);
	void loop();

private:
	drivers::can::CANBus* CANBus;
	osMessageQueueId_t queue;
};

} // namespace tasks
