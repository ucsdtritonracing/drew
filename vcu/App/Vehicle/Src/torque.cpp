#include "torque.hpp"
#include "vehicle_state.hpp"


namespace torque {

float computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps, float motorRPM) {
	// TODO: Compute driver torque request / pedal mapping
	return 0;
}

bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps) {
	// TODO: Compute whether APPS readings are plausible
	return false;
}

} // namespace torque
