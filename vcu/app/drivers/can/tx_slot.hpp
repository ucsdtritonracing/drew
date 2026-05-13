#pragma once

#include "drivers/can/can_utils.hpp"
#include "cmsis_os.h"
#include <stdint.h>

namespace drivers::can {

enum TxPriority : uint8_t {
	CRITICAL	= 0,
	CONTROL		= 1,
	STATUS		= 2,
	TELEMETRY	= 3
};

struct TxSlotHandle {
	uint8_t id;
	bool valid;
};

struct TxSlot {
    uint32_t id;
    TickType_t lastTx;
    TxPriority priority;
	uint8_t data[MAX_CLASSICAL_CAN_DATA_LENGTH];
	uint8_t numBytes;
    bool newMessage;
};

} // namespace drivers::can
