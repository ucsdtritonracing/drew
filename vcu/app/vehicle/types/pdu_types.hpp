#pragma once

#include "drivers/can/can_utils.hpp"
#include <cstddef>
#include <stdint.h>


namespace vehicle::pdu {

constexpr size_t NUM_CHANNELS = 8;

struct State {
	uint16_t measuredCurrent[NUM_CHANNELS];
	enum ErrorStatus {
		OK, OPEN_CIRCUIT, CURRENT_LIMIT_EXCEEDED, SHORT_CIRCUIT, UNKNOWN
	} errorStatuses[NUM_CHANNELS];
};

struct ChannelConfig {
	uint8_t channel;
	float currentLimitAmps;
};

} // namespace vehicle::pdu
