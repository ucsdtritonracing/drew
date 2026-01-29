#include "task_polling.hpp"
#include "vehicle_state.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>

namespace tasks {

void PollingTask::setup() {
	vehicle::vehicleState.setReadyToDrive(false);
}

void PollingTask::loop() {
	uint32_t read = HAL_GPIO_ReadPin(R2D_Button_GPIO_Port, R2D_Button_Pin);
	if (read && !vehicle::vehicleState.readyToDriveButtonPressed) {
		vehicle::vehicleState.setReadyToDrive(!vehicle::vehicleState.getReadyToDrive());
	}

	vehicle::vehicleState.readyToDriveButtonPressed = read;
	osDelay(DELAY);
}

} // namespace tasks
