#pragma once

#include "rtos/task.hpp"


namespace rtos::tasks {

class BroadcastTask : public Task<BroadcastTask, osPriorityHigh, 128> {
public:
	void loop();

private:
	static const uint32_t DELAY = 50;
};

} // namespace rtos::tasks
