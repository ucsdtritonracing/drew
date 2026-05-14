#include "tasks/can_tx.hpp"
#include "drivers/can/can_utils.hpp"
#include "stm32g4xx_hal.h"


namespace rtos::tasks {

void CANTxTask::init(drivers::can::CANBus& canBus) {
	this->canBus = &canBus;
}

void CANTxTask::loop() {
	osThreadFlagsWait(TRY_TX_FLAG, osFlagsWaitAny, TIMEOUT_TICKS);
	canBus->flushTx();
}

} // namespace rtos::tasks
