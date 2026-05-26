#pragma once

#include "vehicle/defaults.hpp"
#include "vehicle/types/configuration_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include <stdint.h>


namespace vehicle {

class VehicleConfiguration {
public:
	static constexpr pdu::ChannelConfig PDU_12V_RIGHT_CHANNEL 		{ 1, 3.0f  };
	static constexpr pdu::ChannelConfig PDU_12V_MAIN_CHANNEL		{ 2, 3.0f  };
	static constexpr pdu::ChannelConfig PDU_TSB_FANS_CHANNEL		{ 3, 3.0f  };
	static constexpr pdu::ChannelConfig PDU_RADIATOR_FANS_CHANNEL	{ 4, 10.0f };
	static constexpr pdu::ChannelConfig PDU_PUMPS_CHANNEL			{ 5, 5.0f  };
	static constexpr pdu::ChannelConfig PDU_BRAKE_LIGHT_CHANNEL		{ 6, 2.0f  };
	static constexpr pdu::ChannelConfig PDU_UNUSED_CHANNEL			{ 7, 0.0f  };
	static constexpr pdu::ChannelConfig PDU_12V_LEFT_CHANNEL 		{ 8, 3.0f  };

	AnalogCalibration app1Thresholds = defaults::APP1_THRESHOLDS;
	AnalogCalibration app2Thresholds = defaults::APP2_THRESHOLDS;
	AnalogCalibration bsefThresholds = defaults::BSEF_THRESHOLDS;
	AnalogCalibration bserThresholds = defaults::BSER_THRESHOLDS;

	float bsefBrakeEngagedThreshold = defaults::BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold = defaults::BSER_BRAKE_ENGAGED_THRESHOLD;

	size_t frontTriggerWheelTeeth	= defaults::FRONT_TRIGGER_WHEEL_TEETH;
	size_t rearTriggerWheelTeeth	= defaults::REAR_TRIGGER_WHEEL_TEETH;
	float frontWheelMetersPerRevolution	= defaults::FRONT_WHEEL_METERS_PER_REVOLUTION;
	float rearWheelMetersPerRevolution	= defaults::REAR_WHEEL_METERS_PER_REVOLUTION;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
