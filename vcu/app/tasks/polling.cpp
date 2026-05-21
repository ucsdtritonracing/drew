#include "tasks/polling.hpp"
#include "vehicle/vehicle_state.hpp"
#include "main.h"
#include "cmsis_os.h"
#include <stdio.h>


namespace rtos::tasks {

void PollingTask::init() {
	r2dbPressedPrev = false;
}

void PollingTask::loop() {
	uint32_t now = HAL_GetTick();
	bool r2dbPressedCurrently = HAL_GPIO_ReadPin(R2D_Button_GPIO_Port, R2D_Button_Pin) == GPIO_PIN_RESET;	// pressed = GND (RESET)

	bool r2dbHeld = false;
	if (r2dbPressedCurrently && !r2dbPressedPrev) {
		r2dbPressTick = now;
	}
	if (r2dbPressedCurrently && (now - r2dbPressTick) > R2DB_HOLD_MS) {
		r2dbHeld = true;
	}

	r2dbPressedPrev = r2dbPressedCurrently;
	vehicle::vehicleState.setReadyToDriveButtonPressed(r2dbHeld);


	GPIO_PinState sdcClosed = HAL_GPIO_ReadPin(SDC_IN_GPIO_Port, SDC_IN_Pin);
	vehicle::vehicleState.setShutdownCircuitClosed(sdcClosed);

	loopTimer.wait();
}

} // namespace rtos::tasks
