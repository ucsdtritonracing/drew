#include "torque.hpp"
#include "vehicle_state.hpp"
#include "vehicle_configuration.hpp"
#include <cmath>

namespace torque {

float computeDriverTorqueRequest(float app) {
	return 1.0f / (1.0f + std::exp(-11.0f * (app - 0.5f)));
}

bool isAPPSPlausible(float app1, float app2) {
	if ((app1 < APPS_DEVIATION_MIN_ACTIVATION_THRESHOLD) || (app2 < APPS_DEVIATION_MIN_ACTIVATION_THRESHOLD)) {
		return true;
	}
	return std::abs(app1 - app2) <= APPS_DEVIATION_MAX_THRESHOLD;
}

bool isAPPSBrakePedalPlausible(bool faultActive, float app, float bsef, float bser) {
	if (!faultActive) {
		return (app <= ABPPC_APP_FAULT_THRESHOLD) ||
			   (bsef <= vehicle::vehicleConfiguration.bsefBrakeEngagedThreshold) ||
			   (bser <= vehicle::vehicleConfiguration.bserBrakeEngagedThreshold);
	} else {
		return (app <= ABPPC_APP_RESET_THRESHOLD);
	}
}

} // namespace torque
