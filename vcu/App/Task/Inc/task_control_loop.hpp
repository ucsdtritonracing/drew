#pragma once
#include "task.hpp"
#include "stm32g4xx_hal.h"

namespace tasks {

static const uint32_t TORQUE_INHIBIT_DELAY = 100;

struct TimedFault {
	bool faultActive = false;
	uint32_t faultTime = 0;
	void update(bool faultStatus) {
		if(faultStatus) {
			if(!faultActive) {
				faultActive = true;
				faultTime = HAL_GetTick();
			}
		} else {
			faultActive = false;
			faultTime = 0;
		}
	}
	bool torqueInhibited() const {
		return faultActive && ((HAL_GetTick() - faultTime) >= TORQUE_INHIBIT_DELAY);
	}
};

class ControlLoopTask : public Task<ControlLoopTask> {
public:
	void loop();

private:
	static const uint32_t CONTROL_LOOP_PERIOD_MS = 				3;
	static constexpr float APPS_PEDAL_TRAVEL_RESET_THRESHOLD = 	0.05;

	TimedFault app1Fault;
	TimedFault app2Fault;
	TimedFault frontBSEFault;
	TimedFault rearBSEFault;
	TimedFault appsPlausibilityFault;

	bool appsBrakePedalPlausibilityFaulted = 					false;
	bool requestZeroTorque;

	void updateAPPSBrakePedalPlausibility();
};

} // namespace tasks
