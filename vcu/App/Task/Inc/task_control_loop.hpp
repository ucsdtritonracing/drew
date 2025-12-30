#pragma once
#include "task.hpp"

namespace tasks {

class ControlLoopTask : public Task<ControlLoopTask> {
public:
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 3;
};

} // namespace tasks
