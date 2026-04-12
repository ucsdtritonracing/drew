#pragma once

#include "stm32g4xx_hal.h"
#include <stdint.h>
#include <cstddef>


namespace drivers::can {

constexpr size_t MAX_CLASSICAL_CAN_DATA_LENGTH = 8;
constexpr uint32_t MAX_CAN_STD_ID = 0x7FF;
constexpr uint32_t MAX_CAN_EXTD_ID = 0x1FFFFFFF;

struct Message {
	FDCAN_RxHeaderTypeDef rxHeader;
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

} // namespace drivers::can
