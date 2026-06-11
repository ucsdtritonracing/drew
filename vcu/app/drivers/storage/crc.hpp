#pragma once

#include <cstddef>
#include <cstdint>


namespace drivers::storage {

uint32_t crc(const uint8_t* data, size_t len);

} // namesapce drivers::storage
