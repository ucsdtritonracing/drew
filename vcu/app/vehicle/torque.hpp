#pragma once

#include "vehicle/vehicle_state.hpp"


namespace torque {

constexpr float MAX_TORQUE_LIMIT_NM = 10;

// T.4.2 Accelerator Pedal Position Sensor - APPS
constexpr float APPS_DEVIATION_MIN_ACTIVATION_THRESHOLD = 0.1;		// T.4.2.3
constexpr float APPS_DEVIATION_MAX_THRESHOLD = 0.1;					// T.4.2.4

// EV.4.7 APPS / Brake Pedal Plausibility Check (ABPPC)
constexpr float ABPPC_APP_FAULT_THRESHOLD = 0.25;					// EV.4.7.1
constexpr float ABPPC_APP_RESET_THRESHOLD = 0.05;					// EV.4.7.2

const uint32_t FAULT_DEBOUNCE_DELAY_MS = 100;						// T.4.2.5 & T.4.3.3

struct TimedFault {
	bool faultActive = false;
	uint32_t faultStartTick = 0;

	void reset() {
		faultActive = false;
		faultStartTick = 0;
	}

	void update(bool faulting, uint32_t currentTick) {
		if (!faulting) {
			faultActive = false;
		} else if (!faultActive) {
			faultActive = true;
			faultStartTick = currentTick;
		}
	}

	bool torqueInhibited(uint32_t currentTick) const {
		return faultActive && ((currentTick - faultStartTick) >= FAULT_DEBOUNCE_DELAY_MS);
	}
};

/*
 * @brief Compute the driver torque request as a scalar from 0 to 1
 *
 * @param app Accelerator pedal position
 */
float computeDriverTorqueRequest(float app);

/*
 * @brief Compute whether accelerator pedal positions are plausible
 *
 * @param app1 APPS1 pedal position
 * @param app2 APPS2 pedal position
 */
bool isAPPSPlausible(float app1, float app2);

/*
 * @brief Compute whether accelerator pedal position and BSE signals are plausible
 *
 * @param faultActive Current fault state
 * @param bsef Front BSE pressure
 * @param bser Rear BSE pressure
 */
bool isAPPSBrakePedalPlausible(bool faultActive, float app, float bsef, float bser);

} // namespace torque
