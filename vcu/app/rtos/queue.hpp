#pragma once

#include "cmsis_os.h"
#include <type_traits>


namespace rtos {

template<typename T, size_t Size>
class Queue {
public:
	void init() {
		if (queueHandle) {
			return;
		}

		const osMessageQueueAttr_t attr = {
		  .name = "CANBus1RxQueue",
		  .cb_mem = &controlBlock,
		  .cb_size = sizeof(controlBlock),
		  .mq_mem = &buffer,
		  .mq_size = sizeof(buffer)
		};
		queueHandle = osMessageQueueNew(sizeof(buffer)/sizeof(T), sizeof(T), &attr);
	}

	osMessageQueueId_t getHandle() {
		return queueHandle;
	}

protected:
	osMessageQueueId_t queueHandle;

private:

	uint8_t buffer[Size * sizeof(T)];
	StaticQueue_t controlBlock;
};

} // namespace rtos
