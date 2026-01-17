#pragma once
#include "task.hpp"
#include "can_bus.hpp"

namespace tasks {

class CANBusTask: public Task<CANBusTask, osPriorityHigh, 512> {
public:
	CANBusTask(const drivers::can::CANBus& CANBus, osMessageQueueId_t queue);
	void loop();

private:
	const drivers::can::CANBus& CANBus;
	osMessageQueueId_t queue;
};

} // namespace tasks
