#pragma once
#include "task.hpp"
#include "can_bus.hpp"

namespace tasks {

class CANBusTask: public Task<CANBusTask, osPriorityHigh, 512> {
public:
	/*
	 * @brief Initialize this CANBus Task.
	 *
	 * @param CANBus The CANBus to attach this task to.
	 * @param queue The CAN message queue to wake up this task.
	 */
	void init(drivers::can::CANBus& CANBus, osMessageQueueId_t queue);
	void loop();

private:
	drivers::can::CANBus* CANBus;
	osMessageQueueId_t queue;
};

} // namespace tasks
