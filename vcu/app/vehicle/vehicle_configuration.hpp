#pragma once

#include "vehicle/defaults.hpp"
#include "vehicle/types/configuration_types.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "vehicle/pedal_map.hpp"


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

	PedalMap pedalMap;

	AnalogCalibration app1Thresholds = defaults::APP1_THRESHOLDS;
	AnalogCalibration app2Thresholds = defaults::APP2_THRESHOLDS;
	AnalogCalibration bsefThresholds = defaults::BSEF_THRESHOLDS;
	AnalogCalibration bserThresholds = defaults::BSER_THRESHOLDS;

	float bsefBrakeEngagedThreshold = defaults::BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold = defaults::BSER_BRAKE_ENGAGED_THRESHOLD;

	float maxTorqueNm = defaults::MAX_TORQUE_NM;

	bool valid();

private:
	static constexpr float MIN_THRESHOLD_VOLTAGE        = 0.0f;
	static constexpr float MAX_THRESHOLD_VOLTAGE        = 5.0f;
	static constexpr float MIN_BRAKE_ENGAGED_THRESHOLD  = 0.0f;
	static constexpr float MAX_BRAKE_ENGAGED_THRESHOLD  = 1.0f;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
