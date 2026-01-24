#pragma once
#include "task.hpp"
#include "can_bus.hpp"

namespace tasks {

class TestTask: public Task<TestTask, osPriorityHigh, 512> {
public:
	TestTask(uint8_t please);
	void loop();

private:
	uint32_t counter = 0;
};

} // namespace tasks
