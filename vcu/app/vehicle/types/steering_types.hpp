#pragma once

#include <stdint.h>


namespace vehicle::steering {

enum Mode {
	CALIBRATED_VALID,
	UNCALIBRATED_VALID,
	FAILURE,
	INVALID
};

struct State {
	uint16_t angleDegrees			= 0;
	uint8_t speedDegreesPerSecond	= 0;
	Mode mode						= INVALID;
};

} // namespace vehicle::steering
