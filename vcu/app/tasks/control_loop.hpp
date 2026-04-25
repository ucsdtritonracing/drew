#pragma once

#include "generics/task.hpp"
#include "vehicle/torque.hpp"
#include "vehicle/types/mode_types.hpp"


namespace tasks {

class ControlLoopTask : public Task<ControlLoopTask> {
public:
	void setup();
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 3;

	struct TransitionInputs {
		bool brakePressed;
		bool readyToDriveButtonPressed;
		bool shutdownCircuitClosed;
	};

	torque::TimedFault app1Fault;
	torque::TimedFault app2Fault;
	torque::TimedFault bsefFault;
	torque::TimedFault bserFault;
	torque::TimedFault appsPlausibilityFault;

	bool appsBrakePedalPlausibilityFaulted;

	const vehicle::Mode getNextMode(vehicle::Mode currentMode, TransitionInputs inputs) const;
	void onEnter(vehicle::Mode mode);
	void runMode(vehicle::Mode mode);
};

} // namespace tasks
