#include "vehicle_configuration.hpp"

namespace vehicle {

VehicleConfiguration vehicleConfiguration{};

VehicleConfiguration::VehicleConfiguration() {
	apps1LoThresholdVolts = INITIAL_APPS1_LO_THRESHOLD_VOLTAGE;
	apps1HiThresholdVolts = INITIAL_APPS1_HI_THRESHOLD_VOLTAGE;
	apps2LoThresholdVolts = INITIAL_APPS2_LO_THRESHOLD_VOLTAGE;
	apps2HiThresholdVolts = INITIAL_APPS2_HI_THRESHOLD_VOLTAGE;

	bsefBrakeEngagedThreshold = INITIAL_BSEF_BRAKE_ENGAGED_THRESHOLD;
	bserBrakeEngagedThreshold = INITIAL_BSER_BRAKE_ENGAGED_THRESHOLD;
}

} // namespace vehicle
