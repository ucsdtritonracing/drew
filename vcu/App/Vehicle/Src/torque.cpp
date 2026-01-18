#include "torque.hpp"
#include "vehicle_state.hpp"
#include <algorithm>

namespace torque {

uint16_t computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps, float torqueCapability) {
	uint16_t torqueRequestNm = static_cast<uint16_t>(std::lroundf(std::min(apps.app1 * MAX_TORQUE_LIMIT, torqueCapability)));
	return torqueRequestNm;
}

bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps) {
	// TODO: Compute whether APPS readings are plausible
	return false;
}

} // namespace torque
