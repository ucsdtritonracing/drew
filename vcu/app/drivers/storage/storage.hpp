#pragma once

#include "vehicle/vehicle_configuration.hpp"
#include "stm32g4xx_hal.h"
#include <type_traits>


namespace drivers::storage {

class ConfigStorage {
public:
	struct Record {
		uint32_t magic;
		vehicle::VehicleConfiguration config;
		uint32_t checksum;
	};

	static constexpr uint32_t CONFIG_FLASH_ADDR	= 0x0807F800; // Last 2KB page
	static constexpr uint32_t CONFIG_MAGIC		= 0xDEADF5AE;

	static bool save(const vehicle::VehicleConfiguration &config);
	static bool load(vehicle::VehicleConfiguration &config);

private:
	static constexpr uint32_t CONFIG_VERSION = 2;
	static constexpr size_t EXPECTED_CONFIG_SIZE = 276;

	static bool eraseAndWrite(const Record &stored);
	static uint32_t computeChecksum(const vehicle::VehicleConfiguration &config);

	static_assert(std::is_trivially_copyable_v<vehicle::VehicleConfiguration>,
			"VehicleConfiguration must be trivially copyable for flash storage");
	static_assert(sizeof(vehicle::VehicleConfiguration) == EXPECTED_CONFIG_SIZE,
			"VehicleConfiguration layout changed, update CONFIG_VERSION");
};

} // namespace drivers::storage
