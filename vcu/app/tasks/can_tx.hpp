#pragma once

#include "rtos/task.hpp"
#include "drivers/can/can_bus.hpp"
#include "cmsis_os.h"


namespace rtos::tasks {

class CANTxTask: public Task<CANTxTask, osPriorityHigh, 512> {
public:
	void init(drivers::can::CANBus& canBus);
	void loop();

	static constexpr uint32_t TRY_TX_FLAG = 0x1;
private:
	static constexpr uint32_t TIMEOUT_MS = 20;
	static constexpr uint32_t TIMEOUT_TICKS = pdMS_TO_TICKS(TIMEOUT_MS);

	drivers::can::CANBus* canBus;
};

} // namespace rtos::tasks
