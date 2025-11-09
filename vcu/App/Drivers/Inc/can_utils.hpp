#pragma once

namespace drivers::CAN {

/**
 * @brief Convert from FDCAN Data Length Code to data length in bytes
 *
 * @param dlc Data length code to convert to data length in bytes.
 */
inline uint8_t numBytes(uint32_t dlc) {
    static const uint8_t dlcTable[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};
    return dlcTable[dlc & 0x0F];
}


} // namespace drivers::CAN
