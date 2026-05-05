#pragma once

#include "generics/task.hpp"
#include "generics/timers.hpp"


namespace tasks {

class PollingTask : public Task<PollingTask, osPriorityLow, 128> {
public:
	void loop();

private:
	static const uint32_t DELAY = 100;
	timers::PeriodicTimer loopTimer{DELAY};
};

} // namespace tasks
