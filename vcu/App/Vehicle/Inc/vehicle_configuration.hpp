#pragma once
#include "stdint.h"

namespace vehicle {

class VehicleConfiguration {
	static constexpr uint8_t PDU_12V_CHANNEL			= 1;
	static constexpr uint8_t PDU_TSSI_RED_CHANNEL		= 2;
	static constexpr uint8_t PDU_TSSI_GREEN_CHANNEL		= 3;
	static constexpr uint8_t PDU_BRAKE_LIGHT_CHANNEL	= 4;
	static constexpr uint8_t PDU_TSB_FANS_CHANNEL		= 5;
	static constexpr uint8_t PDU_PUMPS_CHANNEL			= 6;

	// Default values
	static constexpr float DEFAULT_APPS1_LO_THRESHOLD_VOLTAGE 		= 0.6f;
	static constexpr float DEFAULT_APPS1_HI_THRESHOLD_VOLTAGE 		= 3.1f;
	static constexpr float DEFAULT_APPS2_LO_THRESHOLD_VOLTAGE 		= 0.2f;
	static constexpr float DEFAULT_APPS2_HI_THRESHOLD_VOLTAGE 		= 2.7f;

	static constexpr float DEFAULT_BSEF_LO_THRESHOLD_VOLTAGE 		= 0.5f;
	static constexpr float DEFAULT_BSEF_HI_THRESHOLD_VOLTAGE 		= 4.5f;
	static constexpr float DEFAULT_BSER_LO_THRESHOLD_VOLTAGE 		= 0.5f;
	static constexpr float DEFAULT_BSER_HI_THRESHOLD_VOLTAGE 		= 4.5f;

	static constexpr float DEFAULT_BSEF_BRAKE_ENGAGED_THRESHOLD 	= 0.25;
	static constexpr float DEFAULT_BSER_BRAKE_ENGAGED_THRESHOLD 	= 0.25;

public:
	float apps1LoThresholdVoltage = DEFAULT_APPS1_LO_THRESHOLD_VOLTAGE;
	float apps1HiThresholdVoltage = DEFAULT_APPS1_HI_THRESHOLD_VOLTAGE;
	float apps2LoThresholdVoltage = DEFAULT_APPS2_LO_THRESHOLD_VOLTAGE;
	float apps2HiThresholdVoltage = DEFAULT_APPS2_HI_THRESHOLD_VOLTAGE;

	float bsefLoThresholdVoltage = DEFAULT_BSEF_LO_THRESHOLD_VOLTAGE;
	float bsefHiThresholdVoltage = DEFAULT_BSEF_HI_THRESHOLD_VOLTAGE;
	float bserLoThresholdVoltage = DEFAULT_BSER_LO_THRESHOLD_VOLTAGE;
	float bserHiThresholdVoltage = DEFAULT_BSER_HI_THRESHOLD_VOLTAGE;

	float bsefBrakeEngagedThreshold = DEFAULT_BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold = DEFAULT_BSER_BRAKE_ENGAGED_THRESHOLD;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
