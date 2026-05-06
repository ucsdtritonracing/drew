#pragma once

#include "vehicle/defaults.hpp"
#include "vehicle/types/configuration_types.hpp"
#include <stdint.h>


namespace vehicle {

class VehicleConfiguration {
	static constexpr uint8_t PDU_12V_CHANNEL			= 1;
	static constexpr uint8_t PDU_TSSI_RED_CHANNEL		= 2;
	static constexpr uint8_t PDU_TSSI_GREEN_CHANNEL		= 3;
	static constexpr uint8_t PDU_BRAKE_LIGHT_CHANNEL	= 4;
	static constexpr uint8_t PDU_TSB_FANS_CHANNEL		= 5;
	static constexpr uint8_t PDU_PUMPS_CHANNEL			= 6;

public:
	AnalogCalibration app1Thresholds = defaults::APP1_THRESHOLDS;
	AnalogCalibration app2Thresholds = defaults::APP2_THRESHOLDS;
	AnalogCalibration bsefThresholds = defaults::BSEF_THRESHOLDS;
	AnalogCalibration bserThresholds = defaults::BSER_THRESHOLDS;

	float bsefBrakeEngagedThreshold = defaults::BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold = defaults::BSER_BRAKE_ENGAGED_THRESHOLD;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
