#pragma once

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <cstddef>


namespace drivers::can {

constexpr size_t MAX_CLASSICAL_CAN_DATA_LENGTH = 8;
constexpr uint32_t MAX_CAN_STD_ID = 0x7FF;
constexpr uint32_t MAX_CAN_EXTD_ID = 0x1FFFFFFF;

struct Message {
	uint32_t id;
	uint8_t numBytes;
	uint8_t data[MAX_CLASSICAL_CAN_DATA_LENGTH];
};

/**
 * @brief Convert from FDCAN Data Length Code to data length in bytes
 *
 * @param dlc Data length code to convert to data length in bytes.
 */
inline uint8_t numBytes(uint32_t dlc) {
    static const uint8_t dlcTable[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};
    return dlcTable[dlc & 0x0F];
}

/**
 * @brief Convert from data length in bytes to FDCAN Data Length Code
 *
 * @param numBytes Data length in bytes to convert to data length code.
 */
inline uint8_t dlcFromBytes(uint32_t numBytes) {
	if (numBytes <= 8) {
		return numBytes;
	}

	switch (numBytes) {
	case 12:
		return FDCAN_DLC_BYTES_12;
	case 16:
		return FDCAN_DLC_BYTES_16;
	case 20:
		return FDCAN_DLC_BYTES_20;
	case 24:
		return FDCAN_DLC_BYTES_24;
	case 32:
		return FDCAN_DLC_BYTES_32;
	case 48:
		return FDCAN_DLC_BYTES_48;
	case 64:
		return FDCAN_DLC_BYTES_64;
	default:
		return 0;
	}
}

} // namespace drivers::can
