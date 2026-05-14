#pragma once

#include "vehicle/torque.hpp"
#include "cmsis_os.h"
#include "task.h"
#include <stdint.h>


namespace timers {

class PeriodicTimer {
public:
	PeriodicTimer(TickType_t periodMs)
		: period(pdMS_TO_TICKS(periodMs)), lastWakeTime(0), initialized(false) {}

	void wait() {
		if (!initialized) {
			lastWakeTime = xTaskGetTickCount();
			initialized = true;
		}
		vTaskDelayUntil(&lastWakeTime, period);
	}

private:
	TickType_t period;
	TickType_t lastWakeTime;
	bool initialized;
};

} // namespace timers
