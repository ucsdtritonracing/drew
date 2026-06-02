#pragma once


namespace vehicle {

enum class Mode : uint8_t {
	IDLE			= 0,
	READY_TO_DRIVE	= 1,
	CONFIGURATION	= 2,
};

enum class Strategy : uint8_t {
	DEFAULT			= 0,
	ECO				= 1,
	LAUNCH			= 2,
};

} // namespace vehicle
