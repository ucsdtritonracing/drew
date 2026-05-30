#pragma once

#include "vehicle/types/configuration_types.hpp"


namespace vehicle::defaults {

const AnalogCalibration APP1_THRESHOLDS = {
	.faultThresholds	= {0.2, 0.98},
	.signalThresholds	= {0.672, 0.838}
};
const AnalogCalibration APP2_THRESHOLDS = {
	.faultThresholds	= {0.2, 0.8},
	.signalThresholds	= {0.352, 0.464}
};
const AnalogCalibration BSEF_THRESHOLDS = {
	.faultThresholds	= {0.17, 0.65},
	.signalThresholds	= {0.17, 0.65}
};
const AnalogCalibration BSER_THRESHOLDS = {
	.faultThresholds	= {0.17, 0.65},
	.signalThresholds	= {0.17, 0.65}
};

constexpr float BSEF_BRAKE_ENGAGED_THRESHOLD 	= 0.16;
constexpr float BSER_BRAKE_ENGAGED_THRESHOLD 	= 0.16;

constexpr float MAX_TORQUE_NM					= 15.0f;

constexpr size_t FRONT_TRIGGER_WHEEL_TEETH	= 15;
constexpr size_t REAR_TRIGGER_WHEEL_TEETH	= 24;
constexpr float FRONT_WHEEL_METERS_PER_REVOLUTION = 1.276;
constexpr float REAR_WHEEL_METERS_PER_REVOLUTION = 1.276;

} // namespace vehicle::defaults
