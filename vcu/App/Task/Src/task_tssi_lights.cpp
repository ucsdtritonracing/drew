#include "task_tssi_lights.hpp"
#include "task.hpp"
#include "cmsis_os.h"
#include "main.h"
#include <cstdio>

namespace tasks {

TSSILightsTask::TSSILightsTask() {}

void TSSILightsTask::loop() {
	faultStateBMS = HAL_GPIO_ReadPin(BMS_Fault_GPIO_Port, BMS_Fault_Pin)
			== GPIO_PIN_RESET;
	faultStateIMD = HAL_GPIO_ReadPin(IMD_Fault_GPIO_Port, IMD_Fault_Pin)
			== GPIO_PIN_RESET;
	if (faultStateBMS || faultStateIMD) {
		vehicle::pdu.setCurrentLimit(1, 0); // green light off
		vehicle::pdu.setCurrentLimit(2, 1); // red flash on (hardware implemented)
	} else {
		vehicle::pdu.setCurrentLimit(1, 1); // green light on
		vehicle::pdu.setCurrentLimit(2, 0); // red flash off
	}

	osDelay(250);
}

} // namespace tasks
