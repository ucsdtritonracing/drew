#pragma once
#include "task.hpp"

namespace tasks {

class ControlLoopTask : public Task<ControlLoopTask> {
public:
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 3;
	bool app1Faulted = false;
	uint32_t app1FaultTime = 0;
	bool app2Faulted = false;
	uint32_t app2FaultTime = 0;
	bool bseFrontFaulted = false;
	uint32_t bseFrontFaultTime = 0;
	bool bseRearFaulted = false;
	uint32_t bseRearFaultTime = 0;
	bool appsPlausibilityFaulted = false;
	uint32_t appsPlausibilityFaultTime = 0;
	bool appsBrakePedalPlausibilityFaulted = false;
	bool requestZeroTorque;

	float computeTorqueSecurity();
	void computeFaultInstructions(bool& fault, uint32_t& faultTime);
	void computeFaultInstructions();
	void resetFault(bool& fault, uint32_t& faultTime);
};

} // namespace tasks
