#pragma once

#include "vehicle/types/configuration_types.hpp"
#include "vehicle/defaults.hpp"
#include "vehicle/types/pdu_types.hpp"
#include "vehicle/pedal_map.hpp"
#include <stdint.h>


namespace vehicle {

class VehicleConfiguration {
public:
	const uint32_t version = 2;

	PDUConfig pduConfig;
	PedalsConfig pedalsConfig;
	TorqueConfig torqueConfig;
	WheelsConfig wheelsConfig;

	bool valid() const;
};

extern VehicleConfiguration vehicleConfiguration;

} // namespace vehicle
