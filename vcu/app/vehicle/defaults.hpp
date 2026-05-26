#pragma once

#include "vehicle/types/configuration_types.hpp"


namespace vehicle::defaults {

const AnalogCalibration APP1_THRESHOLDS = {
	.faultThresholds	= {0.3, 3.0},
	.signalThresholds	= {0.88, 1.252}
};
const AnalogCalibration APP2_THRESHOLDS = {
	.faultThresholds	= {0.3, 3.0},
	.signalThresholds	= {0.85, 1.257}
};
const AnalogCalibration BSEF_THRESHOLDS = {
	.faultThresholds	= {0.33, 2.97},
	.signalThresholds	= {0.33, 2.97}
};
const AnalogCalibration BSER_THRESHOLDS = {
	.faultThresholds	= {0.33, 2.97},
	.signalThresholds	= {0.33, 2.97}
};

constexpr float BSEF_BRAKE_ENGAGED_THRESHOLD 	= 0.25;
constexpr float BSER_BRAKE_ENGAGED_THRESHOLD 	= 0.25;

constexpr float MAX_TORQUE_NM					= 10.0f;

constexpr size_t FRONT_TRIGGER_WHEEL_TEETH	= 15;
constexpr size_t REAR_TRIGGER_WHEEL_TEETH	= 24;
constexpr float FRONT_WHEEL_METERS_PER_REVOLUTION = 1.276;
constexpr float REAR_WHEEL_METERS_PER_REVOLUTION = 1.276;

} // namespace vehicle::defaults
