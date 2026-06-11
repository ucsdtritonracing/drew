#pragma once

#include "vehicle/types/configuration_types.hpp"
#include "vehicle/torque.hpp"


namespace vehicle {

bool PedalsConfig::valid() const {
	if (!app1Thresholds.faultThresholds.validRange()  ||
		!app1Thresholds.signalThresholds.validRange() ||
		!app2Thresholds.faultThresholds.validRange()  ||
		!app2Thresholds.signalThresholds.validRange()) {
		return false;
	}
	if (!bsefThresholds.faultThresholds.validRange()  ||
		!bsefThresholds.signalThresholds.validRange() ||
		!bserThresholds.faultThresholds.validRange()  ||
		!bserThresholds.signalThresholds.validRange()) {
		return false;
	}
	if (bsefBrakeEngagedThreshold < 0 || bsefBrakeEngagedThreshold > 1 ||
		bserBrakeEngagedThreshold < 0 || bserBrakeEngagedThreshold > 1) {
		return false;
	}
	return true;
}

bool WheelsConfig::valid() const {
	if (!frontWheelMetersPerRevolution <= 0 || !rearWheelMetersPerRevolution <= 0) {
		return false;
	}
	if (wheelSpeedSensorAlpha <= 0 || wheelSpeedSensorAlpha > 1) {
		return false;
	}
	return true;
}

bool TorqueConfig::valid() const {
	if (maxTorqueNm < 0 || maxTorqueNm > torque::MAX_TORQUE_LIMIT_NM) {
		return false;
	}
	return true;
}

} // namespace vehicle
