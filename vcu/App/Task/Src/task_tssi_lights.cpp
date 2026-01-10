#include "task_tssi_lights.hpp"
#include "task.hpp"
#include "cmsis_os.h"
#include "main.h"
#include <cstdio>

namespace tasks {

TSSILightsTask::TSSILightsTask(const drivers::can::CANBus& CANBus)
		: CANBus(CANBus) {}

void TSSILightsTask::loop() {
	faultStateBMS = HAL_GPIO_ReadPin(BMS_Fault_GPIO_Port, BMS_Fault_Pin) == GPIO_PIN_SET;
	faultStateIMD = HAL_GPIO_ReadPin(IMD_Fault_GPIO_Port, IMD_Fault_Pin) == GPIO_PIN_RESET;
	if(faultStateBMS || faultStateIMD){
		vehicle::pdu.setCurrentLimit(1,0); // green light off
		vehicle::pdu.setCurrentLimit(2, flash ? 0 : 1); // alternate on/off
		flash = !flash;
		printf("FAULT\n");
	}
	else {
		vehicle::pdu.setCurrentLimit(1,1); //ok? green light on
		vehicle::pdu.setCurrentLimit(2,0); // red light off
		printf("OK\n");
	}

	osDelay(250);
}

} // namespace tasks
