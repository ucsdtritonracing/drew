#pragma once

#include "vehicle/types/threshold_types.hpp"
#include "vehicle/types/mode_types.hpp"
#include "vehicle/pedal_map.hpp"
#include "vehicle/defaults.hpp"
#include "drivers/pdu/pdu.hpp"


namespace vehicle {

struct PedalsConfig {
	ThresholdConfig app1Thresholds					= defaults::APP1_THRESHOLDS;
	ThresholdConfig app2Thresholds					= defaults::APP2_THRESHOLDS;
	ThresholdConfig bsefThresholds					= defaults::BSEF_THRESHOLDS;
	ThresholdConfig bserThresholds					= defaults::BSER_THRESHOLDS;

	float bsefBrakeEngagedThreshold					= defaults::BSEF_BRAKE_ENGAGED_THRESHOLD;
	float bserBrakeEngagedThreshold					= defaults::BSER_BRAKE_ENGAGED_THRESHOLD;

	bool valid() const;
};

struct WheelsConfig {
	size_t frontTriggerWheelTeeth					= defaults::FRONT_TRIGGER_WHEEL_TEETH;
	size_t rearTriggerWheelTeeth					= defaults::REAR_TRIGGER_WHEEL_TEETH;
	float frontWheelMetersPerRevolution				= defaults::FRONT_WHEEL_METERS_PER_REVOLUTION;
	float rearWheelMetersPerRevolution				= defaults::REAR_WHEEL_METERS_PER_REVOLUTION;

	float wheelSpeedSensorAlpha						= defaults::WHEEL_SPEED_SENSOR_ALPHA;
	size_t wheelSpeedSensorTimeoutMs				= defaults::WHEEL_SPEED_SENSOR_TIMEOUT_MS;

	bool valid() const;
};

struct TorqueConfig {
	float maxTorqueNm								= defaults::MAX_TORQUE_NM;
	PedalMap pedalMap;

	vehicle::Strategy strategy						= vehicle::Strategy::DEFAULT;

	bool valid() const;
};

struct PDUConfig {
	pdu::ChannelConfig PDU_12V_RIGHT_CHANNEL 		{ 1, 3.0f  };
	pdu::ChannelConfig PDU_12V_MAIN_CHANNEL			{ 2, 3.0f  };
	pdu::ChannelConfig PDU_TSB_FANS_CHANNEL			{ 3, 3.0f  };
	pdu::ChannelConfig PDU_RADIATOR_FANS_CHANNEL	{ 4, 10.0f };
	pdu::ChannelConfig PDU_PUMPS_CHANNEL			{ 5, 5.0f  };
	pdu::ChannelConfig PDU_BRAKE_LIGHT_CHANNEL		{ 6, 2.0f  };
	pdu::ChannelConfig PDU_UNUSED_CHANNEL			{ 7, 0.0f  };
	pdu::ChannelConfig PDU_12V_LEFT_CHANNEL 		{ 8, 3.0f  };

	bool valid() const;
};

} // namespace vehicle
