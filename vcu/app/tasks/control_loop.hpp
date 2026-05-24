#pragma once

#include "rtos/task.hpp"
#include "generics/timers.hpp"
#include "vehicle/torque.hpp"
#include "vehicle/types/mode_types.hpp"


namespace rtos::tasks {

class ControlLoopTask : public Task<ControlLoopTask, osPriorityHigh, 128> {
public:
	void setup();
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 3;
	timers::PeriodicTimer loopTimer{CONTROL_LOOP_PERIOD_MS};

	struct TransitionInputs {
		bool brakePressed;
		bool readyToDriveButtonPressed;
		bool shutdownCircuitClosed;
		bool configurationModeRequested;
	};

	torque::TimedFault app1Fault;
	torque::TimedFault app2Fault;
	torque::TimedFault bsefFault;
	torque::TimedFault bserFault;
	torque::TimedFault appsPlausibilityFault;
	bool appsBrakePedalPlausibilityFaulted;

	const vehicle::Mode getNextMode(vehicle::Mode currentMode, TransitionInputs inputs) const;
	void onEnter(vehicle::Mode mode);
};

} // namespace rtos::tasks
