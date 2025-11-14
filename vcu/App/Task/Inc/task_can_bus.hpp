#pragma once
#include "task.hpp"
#include "can_bus.hpp"

namespace tasks {

class CANBusTask: public Task<CANBusTask> {
public:
	CANBusTask(drivers::CAN::CANBus *CANBus, osMessageQueueId_t queue) :
			CANBus(CANBus), queue(queue) {
	}

	void loop();

private:
	drivers::CAN::CANBus *CANBus;
	osMessageQueueId_t queue;
};

} // namespace tasks
