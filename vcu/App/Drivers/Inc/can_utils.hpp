#pragma once
#include "stm32g4xx_hal.h"

namespace drivers::CAN {

/**
 * @brief Send a CAN frame safely with error checking.
 *
 * @param fdcan Pointer to FDCAN handle.
 * @param txHeader Pointer to prefilled TxHeader.
 * @param data Pointer to payload buffer.
 */
inline void transmit(FDCAN_HandleTypeDef* fdcan, FDCAN_TxHeaderTypeDef* txHeader, const uint8_t* data);

/**
 * @brief Convert from FDCAN Data Length Code to data length in bytes
 *
 * @param dlc Data length code to convert to data length in bytes.
 */
inline uint8_t numBytes(uint32_t dlc);

} // namespace drivers::CAN
