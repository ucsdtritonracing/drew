#include "torque.hpp"
#include "vehicle_state.hpp"
#include <cmath>

namespace torque {

float computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps) {
	// return apps.app1;
	return 1.0f / (1.0f + std::exp(-11.0f * (apps.app1 - 0.5f)));
}

bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps) {
	if ((apps.app1 < 0.1) || (apps.app2 < 0.1)) {
		return true;
	}
	return std::abs(apps.app1 - apps.app2) <= 0.1;
}

bool isAPPSBrakePedalPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps, vehicle::VehicleState::BrakePressures brake) {
	return !((apps.app1 > 0.25) && (brake.front > 0.25));
}

} // namespace torque
