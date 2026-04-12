#include "tasks/polling.hpp"
#include "vehicle/vehicle_state.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>


namespace tasks {

void PollingTask::loop() {
	uint32_t read;

	read = HAL_GPIO_ReadPin(R2D_Button_GPIO_Port, R2D_Button_Pin);
	vehicle::vehicleState.setReadyToDriveButtonPressed(read);

	read = HAL_GPIO_ReadPin(SDC_IN_GPIO_Port, SDC_IN_Pin);
	vehicle::vehicleState.setShutdownCircuitClosed(read);

	osDelay(DELAY);
}

} // namespace tasks
