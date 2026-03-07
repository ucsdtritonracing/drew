#pragma once
#include "task.hpp"
#include "torque.hpp"
#include "stm32g4xx_hal.h"

namespace tasks {

class ControlLoopTask : public Task<ControlLoopTask> {
public:
	void setup();
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 3;

	torque::TimedFault app1Fault;
	torque::TimedFault app2Fault;
	torque::TimedFault bsefFault;
	torque::TimedFault bserFault;
	torque::TimedFault appsPlausibilityFault;

	bool appsBrakePedalPlausibilityFaulted;
};

} // namespace tasks
