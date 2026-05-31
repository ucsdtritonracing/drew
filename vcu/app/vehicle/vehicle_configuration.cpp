#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/torque.hpp"


namespace vehicle {

VehicleConfiguration vehicleConfiguration{};

bool VehicleConfiguration::valid() const {
	if (maxTorqueNm > torque::MAX_TORQUE_LIMIT_NM) {
		return false;
	}

	if (!app1Thresholds.faultThresholds.validRange() ||
		!app1Thresholds.signalThresholds.validRange()) {
		return false;
	}
	if (!app2Thresholds.faultThresholds.validRange() ||
		!app2Thresholds.signalThresholds.validRange()) {
		return false;
	}
	if (!bsefThresholds.faultThresholds.validRange() ||
		!bsefThresholds.signalThresholds.validRange()) {
		return false;
	}
	if (!bserThresholds.faultThresholds.validRange() ||
		!bserThresholds.signalThresholds.validRange()) {
		return false;
	}

	if (bsefBrakeEngagedThreshold < 0 || bsefBrakeEngagedThreshold > 1) {
		return false;
	}
	if (bserBrakeEngagedThreshold < 0 || bserBrakeEngagedThreshold > 1) {
		return false;
	}

	return true;
}

} // namespace vehicle
