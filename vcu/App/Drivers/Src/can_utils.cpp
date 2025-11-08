#include "can_utils.hpp"
#include "main.h"

namespace drivers::CAN {

inline void transmit(FDCAN_HandleTypeDef* fdcan, FDCAN_TxHeaderTypeDef* txHeader, const uint8_t* data) {
    if (HAL_FDCAN_AddMessageToTxFifoQ(fdcan, txHeader, data) != HAL_OK) {
        Error_Handler();
    }
}

inline uint8_t numBytes(uint32_t dlc) {
    static const uint8_t dlcTable[16] = {0,1,2,3,4,5,6,7,8,12,16,20,24,32,48,64};
    return dlcTable[dlc & 0x0F];
}

} // namespace drivers::CAN
