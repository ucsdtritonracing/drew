#pragma once

#include "vehicle_state.hpp"


namespace torque {

static constexpr uint16_t MAX_TORQUE_LIMIT = 90;	// N.m

/*
 * @brief Compute the driver torque request as a scalar from 0 to 1
 *
 * @param apps Accelerator pedal positions
 * @param motorRPM Motor speed in RPM
 */
uint16_t computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps, float torqueCapability);

/*
 * @brief Compute whether accelerator pedal positions are plausible
 *
 * @param apps Accelerator pedal positions
 */
bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps);

} // namespace torque
