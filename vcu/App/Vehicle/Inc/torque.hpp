#pragma once

#include "vehicle_state.hpp"


namespace torque {

static constexpr uint16_t MAX_TORQUE_LIMIT = 90; // N.m

/*
 * @brief Compute the driver torque request as a scalar from 0 to 1
 *
 * @param apps Accelerator pedal positions
 * @param motorRPM Motor speed in RPM
 */
float computeDriverTorqueRequest(vehicle::VehicleState::AcceleratorPedalPositions apps);

/*
 * @brief Compute whether accelerator pedal positions are plausible
 *
 * @param apps Accelerator pedal positions
 */
bool isAPPSPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps);

/*
 * @brief Compute whether accelerator pedal position + BSE signals are plausible
 *
 * @param apps Accelerator pedal positions
 * @param bse Brake Pressure readings
 */
bool isAPPSBrakePedalPlausible(vehicle::VehicleState::AcceleratorPedalPositions apps, vehicle::VehicleState::BrakePressures);

} // namespace torque
