#include "task_can_recovery.hpp"
#include "cmsis_os.h"
#include "stm32g4xx_hal.h"
#include "main.h"

namespace tasks {

void CANRecoveryTask::init(FDCAN_HandleTypeDef &hfdcan1, FDCAN_HandleTypeDef &hfdcan2) {
	this->hfdcan1 = &hfdcan1;
	this->hfdcan2 = &hfdcan2;
}

void CANRecoveryTask::loop() {
	uint32_t flags = osThreadFlagsWait(ANY_CANBUS_BUS_OFF_FLAG, osFlagsWaitAny, osWaitForever);
	osDelay(RETRY_DELAY_MS);
	if (flags & CANBUS1_BUS_OFF_FLAG) {
		HAL_FDCAN_Stop(hfdcan1);
		HAL_FDCAN_Start(hfdcan1);
	}
	if (flags & CANBUS2_BUS_OFF_FLAG) {
		HAL_FDCAN_Stop(hfdcan2);
		HAL_FDCAN_Start(hfdcan2);
	}
}

} // namespace tasks
