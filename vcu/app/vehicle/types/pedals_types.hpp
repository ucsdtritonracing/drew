#pragma once

#include <cstdint>


namespace vehicle::pedals {

struct State {
	float app1 = 0;
	float app2 = 0;
	float bsef = 0;
	float bser = 0;

	uint16_t app1Raw;
	uint16_t app2Raw;
	uint16_t bsefRaw;
	uint16_t bserRaw;

	bool app1Valid = false;
	bool app2Valid = false;
	bool bsefValid = false;
	bool bserValid = false;
};

} // namespace vehicle::pedals
