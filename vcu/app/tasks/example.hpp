#pragma once

#include "rtos/task.hpp"


namespace rtos::tasks {

class ExampleTask : public Task<ExampleTask> {
public:
	void setup();
	void loop();

private:
	static const uint32_t DELAY = 1000;
};

} // namespace rtos::tasks
