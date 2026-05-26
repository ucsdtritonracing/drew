#pragma once

#include "rtos/task.hpp"


namespace rtos::tasks {

class StartupTask : public Task<StartupTask, osPriorityLow, 128> {
public:
	void setup();
	void loop();

private:
	static const uint32_t STARTUP_DELAY = 10000;
};

} // namespace rtos::tasks

