#pragma once

#include "rtos/task.hpp"
#include "generics/timers.hpp"
#include "FreeRTOS.h"


namespace rtos::tasks {

class PollingTask : public Task<PollingTask, osPriorityLow, 128> {
public:
	void init();
	void loop();

private:
	static constexpr uint32_t R2DB_HOLD_MS = 1000;
	bool r2dbPressedPrev;
	uint32_t r2dbPressTick;


	static constexpr uint32_t DELAY = 100;
	timers::PeriodicTimer loopTimer{DELAY};
};

} // namespace rtos::tasks
