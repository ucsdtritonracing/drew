#pragma once
#include "stm32g4xx_hal.h"

namespace drivers::CAN {

constexpr uint32_t MAX_CLASSICAL_CAN_DATA_LENGTH = 8;
constexpr uint32_t MAX_CAN_STD_ID = 0x7FF;

struct Message;
struct HandlerEntry;
using CANHandler = void (*)(void*, const Message&);

struct Message {
	FDCAN_RxHeaderTypeDef rxHeader;
	uint8_t numBytes;
	uint8_t data[MAX_CLASSICAL_CAN_DATA_LENGTH];
};

struct HandlerEntry {
	void* instance;
	uint32_t id;
	CANHandler callback;
};

class CANBus {
public:
    CANBus(FDCAN_HandleTypeDef *fdcan) : fdcan(fdcan) {}

    void addMessageHandler(void *instance, uint32_t id, CANHandler callback);
    void processMessage(Message *message) const;
    void transmit(uint32_t id, const uint8_t *data, uint32_t dlc) const;

private:
	static constexpr size_t MAX_HANDLERS = 32;
    HandlerEntry handlers[MAX_HANDLERS];
    size_t numHandlers = 0;

    FDCAN_HandleTypeDef* fdcan;
};

} // namespace drivers::CAN
