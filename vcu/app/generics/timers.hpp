#pragma once

#include "vehicle/torque.hpp"
#include <stdint.h>


namespace timers {

struct TimedPulse {
	bool active = false;
	uint32_t startTick = 0;
	uint32_t duration = 0;

	void trigger(uint32_t currentTick, uint32_t durationMs) {
		active = true;
		startTick = currentTick;
		duration = durationMs;
	}

	void update(uint32_t currentTick) {
		if (active && (currentTick - startTick >= duration)) {
			active = false;
		}
	}

	bool isActive() const {
		return active;
	}
};

} // namespace timers
