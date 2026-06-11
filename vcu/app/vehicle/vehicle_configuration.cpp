#include "vehicle/vehicle_configuration.hpp"
#include "vehicle/torque.hpp"


namespace vehicle {

VehicleConfiguration vehicleConfiguration{};

bool VehicleConfiguration::valid() const {
	if (!pedalsConfig.valid()) {
		return false;
	}
	if (!torqueConfig.valid()) {
		return false;
	}
	if (!wheelsConfig.valid()) {
		return false;
	}
	if (!pduConfig.valid()) {
		return false;
	}

	return true;
}

} // namespace vehicle
