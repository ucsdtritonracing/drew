#pragma once
#include <task.hpp>

namespace tasks {

class ExampleTask : public Task<ExampleTask> {
public:
	void setup();
	void loop();

private:
	static const uint32_t DELAY = 1000;
	int counter = 0;
};

} // namespace tasks
