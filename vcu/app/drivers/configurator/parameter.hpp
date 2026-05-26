#pragma once

#include <stdint.h>


namespace drivers::configurator {

enum class Type: uint8_t {
	VOLTS_FP32					= 0x00,
	SCALAR_FP32					= 0x01,
	SCALAR_U32					= 0x02,
	SCALAR_I32					= 0x03,
};

enum class ThresholdIndex: uint8_t {
	FAULT_LOW					= 0x00,
	FAULT_HIGH					= 0x01,
	SIGNAL_LOW					= 0x02,
	SIGNAL_HIGH					= 0x03,
	ENGAGE						= 0x04,
};

enum class Parameter: uint16_t {
	INVALID						= 0x0000,
	APP1_THRESHOLD				= 0x0001,
	APP2_THRESHOLD				= 0x0002,
	BSEF_THRESHOLD				= 0x0003,
	BSER_THRESHOLD				= 0x0004,
	MAXIMUM_TORQUE_REQUEST_NM	= 0x0005,
	PEDAL_MAP					= 0x0006,
};

} // namespace drivers::configurator
