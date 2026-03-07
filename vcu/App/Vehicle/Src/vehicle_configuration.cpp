#include "vehicle_configuration.hpp"

namespace vehicle {

VehicleConfiguration vehicleConfiguration{};

VehicleConfiguration::VehicleConfiguration() {
	bsefBrakeEngagedThreshold = INITIAL_BSEF_BRAKE_ENGAGED_THRESHOLD;
	bserBrakeEngagedThreshold = INITIAL_BSER_BRAKE_ENGAGED_THRESHOLD;
}

} // namespace vehicle
