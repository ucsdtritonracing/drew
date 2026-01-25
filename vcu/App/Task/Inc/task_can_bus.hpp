#pragma once
#include "task.hpp"
#include "can_bus.hpp"

namespace tasks {

class CANBusTask: public Task<CANBusTask, osPriorityNormal, 256> {
public:
	void init(drivers::can::CANBus* CANBus, osMessageQueueId_t queue);
	void loop();

private:
	drivers::can::CANBus* CANBus;
	osMessageQueueId_t queue;
};

} // namespace tasks
