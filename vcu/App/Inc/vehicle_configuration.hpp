#pragma once
#include "stdint.h"

namespace vehicle {

struct VehicleConfiguration {
	constexpr uint8_t PDU_12V_CHANNEL 			= 1;
	constexpr uint8_t PDU_TSSI_RED_CHANNEL 		= 2;
	constexpr uint8_t PDU_TSSI_GREEN_CHANNEL 	= 3;
	constexpr uint8_t PDU_BRAKE_LIGHT_CHANNEL 	= 4;
	constexpr uint8_t PDU_TSB_FANS_CHANNEL 		= 5;
	constexpr uint8_t PDU_PUMPS_CHANNEL 		= 6;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
