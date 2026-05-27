#pragma once

#include <stdint.h>


namespace drivers::configurator {

enum class ThresholdType: uint16_t {
	FAULT_LOW					= 0x0000,
	FAULT_HIGH					= 0x0001,
	SIGNAL_LOW					= 0x0002,
	SIGNAL_HIGH					= 0x0003,
	ENGAGE						= 0x0004,
};

enum class Parameter: uint16_t {
	APP1_THRESHOLD				= 0x0001,
	APP2_THRESHOLD				= 0x0002,
	BSEF_THRESHOLD				= 0x0003,
	BSER_THRESHOLD				= 0x0004,
	MAX_TORQUE_REQUEST			= 0x0005,
	PEDAL_MAP					= 0x0006,
	SAS_ZER0					= 0x0007,
};

} // namespace drivers::configurator
