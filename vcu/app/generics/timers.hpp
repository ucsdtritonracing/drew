#pragma once

#include "vehicle/torque.hpp"
#include "cmsis_os.h"
#include "task.h"
#include <stdint.h>


namespace timers {

class TimedPulse {
public:
	void trigger(uint32_t currentTick, uint32_t durationMs) {
		active = true;
		startTick = currentTick;
		duration = durationMs;
	}

	void update(uint32_t currentTick) {
		if (active && (currentTick - startTick >= duration)) {
			active = false;
		}
	}

	bool isActive() const {
		return active;
	}

private:
	bool active = false;
	uint32_t startTick = 0;
	uint32_t duration = 0;
};


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
