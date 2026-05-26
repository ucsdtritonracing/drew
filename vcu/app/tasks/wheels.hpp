#pragma once

#include "rtos/task.hpp"


namespace rtos::tasks {

class WheelsTask : public Task<WheelsTask, osPriorityNormal, 128> {
public:
	void loop();

	static constexpr uint32_t INTERVAL_MS = 50;
};

} // namespace rtos::tasks
