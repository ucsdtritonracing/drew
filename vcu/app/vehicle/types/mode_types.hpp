#pragma once


namespace vehicle {

enum class Mode : uint8_t {
	IDLE			= 0,
	READY_TO_DRIVE	= 1,
	CONFIGURATION	= 2,
};

} // namespace vehicle
