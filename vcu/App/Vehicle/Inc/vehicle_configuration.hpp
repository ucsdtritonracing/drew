#pragma once
#include "stdint.h"

namespace vehicle {

class VehicleConfiguration {
	static constexpr uint8_t PDU_12V_CHANNEL 			= 1;
	static constexpr uint8_t PDU_TSSI_RED_CHANNEL 		= 2;
	static constexpr uint8_t PDU_TSSI_GREEN_CHANNEL 	= 3;
	static constexpr uint8_t PDU_BRAKE_LIGHT_CHANNEL 	= 4;
	static constexpr uint8_t PDU_TSB_FANS_CHANNEL 		= 5;
	static constexpr uint8_t PDU_PUMPS_CHANNEL 			= 6;

	static constexpr float INITIAL_BSEF_BRAKE_ENGAGED_THRESHOLD = 0.25;
	static constexpr float INITIAL_BSER_BRAKE_ENGAGED_THRESHOLD = 0.25;

public:
	float bsefBrakeEngagedThreshold;
	float bserBrakeEngagedThreshold;

	VehicleConfiguration();
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
