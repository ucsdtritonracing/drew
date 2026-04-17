#pragma once

#include "generics/task.hpp"
#include "vehicle/torque.hpp"


namespace tasks {

class ControlLoopTask : public Task<ControlLoopTask, osPriorityHigh, 128> {
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
