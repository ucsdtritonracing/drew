#pragma once

#include "drivers/can/can_bus.hpp"
#include "drivers/can/can_peripheral.hpp"
#include "drivers/broadcaster/broadcaster_types.hpp"
#include "stm32g4xx_hal_fdcan.h"
#include "vehicle/types/wheels_types.hpp"
#include "vehicle/types/pedals_types.hpp"


namespace drivers::broadcaster {

class Broadcaster: public drivers::can::CANPeripheral<Broadcaster> {
public:
	Broadcaster(drivers::can::CANBus &canBus);

	void broadcastWheelsMessage(const vehicle::wheels::State& wheels);
	void broadcastBrakesMessage(const vehicle::pedals::State& pedals);
	void broadcastAPPMessage(const vehicle::pedals::State& pedals);
	void broadcastFlagsMessage(Flags flags);

private:
	uint8_t txData[drivers::can::MAX_CLASSICAL_CAN_DATA_LENGTH] = {};

	static constexpr float WHEELS_SCALE					= 100.0f;
	static constexpr float PEDALS_SCALE					= 10.0f;

	static constexpr size_t FLAGS_R2DB_PRESSED_BIT		= 0;
	static constexpr size_t FLAGS_SDC_CLOSED_BIT		= 1;
	static constexpr size_t FLAGS_R2D_ENABLED_BIT		= 2;
	static constexpr size_t FLAGS_APP_FAULT_BIT			= 3;
	static constexpr size_t FLAGS_ABPPC_FAULT_BIT		= 4;

	static constexpr size_t APP_APP1_START				= 0;
	static constexpr size_t APP_APP2_START				= 2;
	static constexpr size_t APP_FLAGS_START				= 4;
	static constexpr size_t APP_APP1_VALID_BIT			= 0;
	static constexpr size_t APP_APP2_VALID_BIT			= 1;

	static constexpr size_t BRAKES_BSEF_START			= 0;
	static constexpr size_t BRAKES_BSER_START			= 2;
	static constexpr size_t BRAKES_FLAGS_START			= 4;
	static constexpr size_t BRAKES_BSEF_VALID_BIT		= 0;
	static constexpr size_t BRAKES_BSER_VALID_BIT		= 1;

	static constexpr size_t WHEELS_FR_START				= 0;
	static constexpr size_t WHEELS_FL_START				= 2;
	static constexpr size_t WHEELS_RR_START				= 4;
	static constexpr size_t WHEELS_RL_START				= 6;

	static constexpr uint8_t DLC_WHEELS_MESSAGE			= FDCAN_DLC_BYTES_8;
	static constexpr uint8_t DLC_BRAKES_MESSAGE			= FDCAN_DLC_BYTES_5;
	static constexpr uint8_t DLC_APP_MESSAGE			= FDCAN_DLC_BYTES_5;
	static constexpr uint8_t DLC_IMU_MESSAGE			= FDCAN_DLC_BYTES_8;
	static constexpr uint8_t DLC_FLAGS_MESSAGE			= FDCAN_DLC_BYTES_1;

	static constexpr uint32_t CAN_ID_WHEELS_MESSAGE		= 0x12A;
	static constexpr uint32_t CAN_ID_BRAKES_MESSAGE		= 0x12B;
	static constexpr uint32_t CAN_ID_APP_MESSAGE		= 0x12C;
	static constexpr uint32_t CAN_ID_IMU_MESSAGE		= 0x12E;
	static constexpr uint32_t CAN_ID_FLAGS_MESSAGE		= 0x12F;

	static constexpr uint16_t BYTE_MASK					= 0xFF;
};

} // namespace drivers::broadcaster
