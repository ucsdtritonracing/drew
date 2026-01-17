#pragma once

#include "vehicle_state.hpp"


namespace torque {

/*
 * @brief Compute the driver torque request as a scalar from 0 to 1
 *
 * @param apps Accelerator pedal positions
 * @param motorRPM Motor speed in RPM
 */
float computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps, float motorRPM);

/*
 * @brief Compute whether accelerator pedal positions are plausible
 *
 * @param apps Accelerator pedal positions
 */
bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps);

} // namespace torque
