#pragma once

#include "rtos/task.hpp"


namespace rtos::tasks {

class BroadcastTask : public Task<BroadcastTask> {
public:
	void loop();

private:
	static const uint32_t DELAY = 100;
};

} // namespace rtos::tasks
