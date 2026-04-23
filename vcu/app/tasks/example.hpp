#pragma once

#include "generics/task.hpp"


namespace tasks {

class ExampleTask : public Task<ExampleTask> {
public:
	void setup();
	void loop();

private:
	static const uint32_t DELAY = 1000;
};

} // namespace tasks
