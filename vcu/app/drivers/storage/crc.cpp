#include "crc.hpp"
#include "stm32g4xx_hal.h"

extern CRC_HandleTypeDef hcrc;


namespace drivers::storage {

uint32_t crc(const uint8_t* data, size_t len) {
    return HAL_CRC_Calculate(
        &hcrc,
        reinterpret_cast<uint32_t*>(const_cast<uint8_t*>(data)),
        len
    );
}

} // namespace drivers::storage
