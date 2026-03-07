#pragma once
#include "stdint.h"

namespace vehicle {

class VehicleConfiguration {
	static constexpr uint8_t PDU_12V_CHANNEL 			= 1;
	static constexpr uint8_t PDU_BRAKE_LIGHT_CHANNEL 	= 4;
	static constexpr uint8_t PDU_TSB_FANS_CHANNEL 		= 5;
	static constexpr uint8_t PDU_RADIATION_FANS_CHANNEL	= 7;
	static constexpr uint8_t PDU_PUMPS_CHANNEL 			= 6;

	static constexpr float INITIAL_APPS1_LO_THRESHOLD_VOLTAGE 	= 0.6f;
	static constexpr float INITIAL_APPS1_HI_THRESHOLD_VOLTAGE 	= 3.1f;
	static constexpr float INITIAL_APPS2_LO_THRESHOLD_VOLTAGE 	= 0.2f;
	static constexpr float INITIAL_APPS2_HI_THRESHOLD_VOLTAGE 	= 2.7f;

	static constexpr float INITIAL_BSEF_LO_THRESHOLD_VOLTAGE 	= 0.5f;
	static constexpr float INITIAL_BSEF_HI_THRESHOLD_VOLTAGE 	= 4.5f;
	static constexpr float INITIAL_BSER_LO_THRESHOLD_VOLTAGE 	= 0.5f;
	static constexpr float INITIAL_BSER_HI_THRESHOLD_VOLTAGE 	= 4.5f;

	static constexpr float INITIAL_BSEF_BRAKE_ENGAGED_THRESHOLD = 0.25;
	static constexpr float INITIAL_BSER_BRAKE_ENGAGED_THRESHOLD = 0.25;

public:
	float apps1LoThresholdVoltage = INITIAL_APPS1_LO_THRESHOLD_VOLTAGE;
	float apps1HiThresholdVoltage = INITIAL_APPS1_HI_THRESHOLD_VOLTAGE;
	float apps2LoThresholdVoltage = INITIAL_APPS2_LO_THRESHOLD_VOLTAGE;
	float apps2HiThresholdVoltage = INITIAL_APPS2_HI_THRESHOLD_VOLTAGE;

	float bsefLoThresholdVoltage = INITIAL_BSEF_LO_THRESHOLD_VOLTAGE;
	float bsefHiThresholdVoltage = INITIAL_BSEF_HI_THRESHOLD_VOLTAGE;
	float bserLoThresholdVoltage = INITIAL_BSER_LO_THRESHOLD_VOLTAGE;
	float bserHiThresholdVoltage = INITIAL_BSER_HI_THRESHOLD_VOLTAGE;

	float bsefBrakeEngagedThreshold = INITIAL_BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold = INITIAL_BSER_BRAKE_ENGAGED_THRESHOLD;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
